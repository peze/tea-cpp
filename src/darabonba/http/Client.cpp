#include <darabonba/Core.hpp>
#include <darabonba/Env.hpp>
#include <darabonba/Exception.hpp>
#include <darabonba/http/Client.hpp>
#include <darabonba/http/Curl.hpp>
#include <darabonba/http/MCurlResponse.hpp>
#include <darabonba/http/Request.hpp>
#include <darabonba/http/Response.hpp>
#include <darabonba/http/URL.hpp>

using std::string;

namespace Darabonba {

namespace Http {

std::unique_ptr<CURLM, Curl::CURLMDeleterWithGlobal> Client::defaultMCurl_ =
    []() {
      curl_global_init(CURL_GLOBAL_ALL);
      return std::unique_ptr<CURLM, Curl::CURLMDeleterWithGlobal>(
          curl_multi_init());
    }();

std::shared_ptr<Response>
Client::doAction(const Request &request, const RuntimeOptions &runtimeOptions) {
  CURL *curl = curl_easy_init();
  auto resp = std::make_shared<MCurlResponse>(mCurlHandle_, curl);

  long connect_timeout = runtimeOptions.connectTimeout();
  long read_timeout = runtimeOptions.readTimeout();
  bool ignore_SSL = runtimeOptions.ignoreSSL();
  string httpProxy = runtimeOptions.httpProxy();
  string httpsProxy = runtimeOptions.httpsProxy();
  string noProxy = runtimeOptions.noProxy();

  // set request method
  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, request.strMethod().c_str());
  // set request url
  curl_easy_setopt(curl, CURLOPT_URL,
                   static_cast<string>(request.url()).c_str());

  // set request header
  curl_slist *list = Curl::setCurlHeader(curl, request.header());

  // set the storage of request body
  Curl::setCurlRequestBody(curl, request.body());

  // set SSL
  if (ignore_SSL) {
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
  } else {
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
  }

  // set timeout
  if (connect_timeout > 0) {
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, connect_timeout);
  }
  if (read_timeout > 0) {
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, read_timeout);
  }

  // set proxy
  if (!httpProxy.empty()) {
    curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_HTTP);
    Curl::setCurlProxy(curl, httpProxy);
  }
  if (!httpsProxy.empty()) {
    curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_HTTPS);
    Curl::setCurlProxy(curl, httpsProxy);
  }
  if (!noProxy.empty()) {
    curl_easy_setopt(curl, CURLOPT_NOPROXY, noProxy.c_str());
  }

  // set the storage of response header
  curl_easy_setopt(curl, CURLOPT_HEADERDATA, &(resp->header()));
  // set how to receive response header
  curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, Curl::writeHeader);

  // set the storage of response body
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, resp->body().get());
  // set how to receive response body
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Curl::recvBody);

  // debug
  string debug = Darabonba::Env::getEnv("DEBUG");
  if (debug == "sdk") {
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
  }

  CURLMcode res;

  int still_running;
  do {
    if (static_cast<MCurlResponse::Body *>(resp->body().get())->ready()) {
      break;
    }
    // todo
    Core::sleep(1);
    res = curl_multi_perform(mCurlHandle_, &still_running);
    if (res) {
      string message = string("curl failed, code_") + std::to_string((int)res);
      throw RetryableException(
          {{"code", "DaraRequestError"}, {"message", message}});
    }
  } while (still_running);

  curl_slist_free_all(list);
  return resp;
}
} // namespace Http
} // namespace Darabonba