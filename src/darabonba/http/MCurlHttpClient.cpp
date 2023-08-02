#include <darabonba/http/Curl.hpp>
#include <darabonba/http/MCurlHttpClient.hpp>
#include <darabonba/http/MCurlResponseBody.hpp>
#include <mutex>

namespace Darabonba {
namespace Http {

std::future<std::shared_ptr<Response>>
MCurlHttpClient::makeRequest(const Request &request,
                             const RuntimeOptions &options) {
  if (!running_ || !mCurl_) {
    std::promise<std::shared_ptr<Response>> promise;
    promise.set_value(nullptr);
    return promise.get_future();
  }
  auto easyHandle = curl_easy_init();
  if (!easyHandle) {
    std::promise<std::shared_ptr<Response>> promise;
    promise.set_value(nullptr);
    return promise.get_future();
  }

  // process the runtime options
  // ssl
  if (options.ignoreSSL()) {
    curl_easy_setopt(easyHandle, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(easyHandle, CURLOPT_SSL_VERIFYHOST, 2L);
  } else {
    curl_easy_setopt(easyHandle, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(easyHandle, CURLOPT_SSL_VERIFYHOST, 0L);
  }
  // timeout
  long connectTimeout = options.connectTimeout();
  if (connectTimeout > 0) {
    curl_easy_setopt(easyHandle, CURLOPT_CONNECTTIMEOUT_MS, connectTimeout);
  }
  long readTimeout = options.readTimeout();
  if (readTimeout > 0) {
    curl_easy_setopt(easyHandle, CURLOPT_TIMEOUT_MS, readTimeout);
  }
  // set proxy
  // todo sock5
  std::string httpProxy = options.httpProxy();
  if (!httpProxy.empty()) {
    curl_easy_setopt(easyHandle, CURLOPT_PROXYTYPE, CURLPROXY_HTTP);
    Curl::setCurlProxy(easyHandle, httpProxy);
  }
  std::string httpsProxy = options.httpsProxy();
  if (!httpsProxy.empty()) {
    curl_easy_setopt(easyHandle, CURLOPT_PROXYTYPE, CURLPROXY_HTTPS);
    Curl::setCurlProxy(easyHandle, httpsProxy);
  }
  std::string noProxy = options.noProxy();
  if (!noProxy.empty()) {
    curl_easy_setopt(easyHandle, CURLOPT_NOPROXY, noProxy.c_str());
  }

  // set request method
  curl_easy_setopt(easyHandle, CURLOPT_CUSTOMREQUEST,
                   request.strMethod().c_str());
  // set request url
  curl_easy_setopt(easyHandle, CURLOPT_URL,
                   static_cast<std::string>(request.url()).c_str());

  // set the storage of request body
  Curl::setCurlRequestBody(easyHandle, request.body());

  // init header
  auto curlStorage = std::unique_ptr<CurlStorage>(new CurlStorage{
      .easyHandle = easyHandle,
      .reqHeader = Curl::setCurlHeader(easyHandle, request.header()),
      .reqBody = request.body(),
      .resp = std::make_shared<Response>()});

  // set response body
  // todo: custom response body by user
  auto body = new MCurlResponseBody();
  body->easyHandle_ = easyHandle;
  body->client_ = this;
  auto &resp = curlStorage->resp;
  resp->setBody(std::shared_ptr<IOStream>(body));

  // set the storage of response header
  curl_easy_setopt(easyHandle, CURLOPT_HEADERDATA, &(resp->header()));
  // set how to receive response header
  curl_easy_setopt(easyHandle, CURLOPT_HEADERFUNCTION, Curl::writeHeader);
  // set the storage of response body
  curl_easy_setopt(easyHandle, CURLOPT_WRITEDATA, curlStorage.get());
  // set how to receive response body
  curl_easy_setopt(easyHandle, CURLOPT_WRITEFUNCTION, recvBody);

  auto ret = curlStorage->promise.get_future();
  {
    std::lock_guard<Lock::SpinLock> guard(reqLock_);
    reqQueue_.emplace_back(std::move(curlStorage));
    ++reqQueueSize_;
  }
  // wake the curl_multi_poll
  curl_multi_wakeup(mCurl_);
  return ret;
}

void MCurlHttpClient::perform() {
  while (running_) {
    if (reqQueueSize_) {
      decltype(reqQueue_) reqQueue;
      {
        std::lock_guard<Lock::SpinLock> guard(reqLock_);
        reqQueue = std::move(reqQueue_);
        reqQueueSize_ = 0;
      }
      while (!reqQueue.empty()) {
        auto storage = std::move(reqQueue.front());
        reqQueue.pop_front();
        // add the easy_curl to multi_curl
        curl_multi_add_handle(mCurl_, storage->easyHandle);
        runningCurl_[storage->easyHandle] = std::move(storage);
      }
    }
    if (continueReadingQueueSize_) {
      decltype(continueReadingQueue_) continueReadingQueue;
      {
        std::lock_guard<Lock::SpinLock> guard(continueReadingLock_);
        continueReadingQueue = std::move(continueReadingQueue_);
        continueReadingQueueSize_ = 0;
      }
      for (auto easyHandle : continueReadingQueue) {
        // todo 加一个判断是否为当前所管理的 easy handle
        // set continue reading
        curl_easy_pause(easyHandle, CURLPAUSE_CONT);
      }
    }
    auto code = curl_multi_poll(mCurl_, nullptr, 0, WAIT_MS, nullptr);
    if (code != CURLM_OK) {
      // todo: handle error
      continue;
    }
    int running_handles;
    curl_multi_perform(mCurl_, &running_handles);

    int msgs_in_queue = 0;
    CURLMsg *msg = nullptr;

    while ((msg = curl_multi_info_read(mCurl_, &msgs_in_queue)) != nullptr) {
      if (msg->msg == CURLMSG_DONE) {
        auto easyHandle = msg->easy_handle;
        auto curlStorage = std::move(runningCurl_[easyHandle]);
        runningCurl_.erase(easyHandle);

        auto body =
            dynamic_cast<MCurlResponseBody *>(curlStorage->resp->body().get());
        if (body) {
          if (!body->ready()) {
            setResponseReady(curlStorage.get());
          }
          // response body has been fully received
          body->done_ = true;
          body->doneCV_.notify_one();
        }
        // remove the easy hanle
        curl_multi_remove_handle(mCurl_, easyHandle);
        curl_easy_cleanup(easyHandle);
      }
    }
  }
}

bool MCurlHttpClient::start() {
  if (!mCurl_ || running_)
    return false;
  running_ = true;
  performThread_ = std::thread(std::bind(&MCurlHttpClient::perform, this));
  return true;
}

bool MCurlHttpClient::stop() {
  if (!running_)
    return false;
  running_ = false;
  // stopCV_.wait();
  return true;
}

void MCurlHttpClient::close() {
  while (!reqQueue_.empty()) {
    auto storage = std::move(reqQueue_.front());
    reqQueue_.pop_front();
    curl_slist_free_all(storage->reqHeader);
    curl_easy_cleanup(storage->easyHandle);
  }
  for (auto &p : runningCurl_) {
    curl_slist_free_all(p.second->reqHeader);
    curl_multi_remove_handle(mCurl_, p.first);
    curl_easy_cleanup(p.first);
  }
  runningCurl_.clear();
}

bool MCurlHttpClient::addContinueReadingHandle(CURL *easyHandle) {
  if (!running_ || !mCurl_ || !easyHandle)
    return false;
  {
    std::lock_guard<Lock::SpinLock> guard(continueReadingLock_);
    continueReadingQueue_.emplace_back(easyHandle);
    ++continueReadingQueueSize_;
  }
  curl_multi_wakeup(mCurl_);
  return true;
}

bool MCurlHttpClient::setResponseReady(CurlStorage *curlStorage) {
  // clear the request header
  curl_slist_free_all(curlStorage->reqHeader);
  curlStorage->reqHeader = nullptr;
  // clear the request body
  curlStorage->reqBody = nullptr;

  // set the status code
  long responseCode;
  curl_easy_getinfo(curlStorage->easyHandle, CURLINFO_RESPONSE_CODE,
                    &responseCode);
  curlStorage->resp->setStatusCode(responseCode);
  // set ready
  static_cast<MCurlResponseBody *>(curlStorage->resp->body().get())->ready_ =
      true;
  curlStorage->promise.set_value(curlStorage->resp);
  return true;
}

size_t MCurlHttpClient::recvBody(char *buffer, size_t size, size_t nmemb,
                                 void *userdata) {
  auto curlStorage = static_cast<CurlStorage *>(userdata);
  auto body =
      dynamic_cast<MCurlResponseBody *>(curlStorage->resp->body().get());
  if (!body)
    return 0;
  if (body->readableSize() >= body->maxSize()) {
    return CURL_WRITEFUNC_PAUSE;
  }
  auto expectSize = size * nmemb;
  auto realSize = body->write(buffer, expectSize);
  body->readableSize_ += realSize;
  body->streamCV_.notify_one();

  if (!body->ready()) {
    setResponseReady(curlStorage);
  }

  return realSize;
}

} // namespace Http
} // namespace Darabonba