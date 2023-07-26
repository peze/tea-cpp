#include <atomic>
#include <condition_variable>
#include <darabonba/RuntimeOptions.hpp>
#include <darabonba/http/Curl.hpp>
#include <darabonba/http/MCurlResponseBody.hpp>
#include <darabonba/http/Request.hpp>
#include <darabonba/http/Response.hpp>
#include <darabonba/lock/SpinLock.hpp>
#include <future>
#include <list>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>

namespace Darabonba {
namespace Http {

class MCurlResponseBody;

class MCurlHttpClient {
  friend class MCurlResponseBody;

public:
  MCurlHttpClient() : mCurl_(curl_multi_init()) {}
  ~MCurlHttpClient() {
    stop();
    // todo 这里还是等所有跑完
    close();
    curl_multi_cleanup(mCurl_);
    mCurl_ = nullptr;
  }

  std::future<std::shared_ptr<Response>>
  makeRequest(const Request &request, const RuntimeOptions &options = {});

  /**
   * @brief Start a background thread to handle network IO
   */
  bool start();

  /**
   * @brief Stop the backgroud thread.
   * @note All existing links will be closed.
   */
  bool stop();

protected:
  enum { WAIT_MS = 2000 };

  struct CurlStorage {
  public:
    CURL *easyHandle;
    // request body
    std::shared_ptr<IStream> reqBody;
    // request header
    curl_slist *reqHeader;
    // http response
    std::shared_ptr<Response> resp;

    std::promise<std::shared_ptr<Response>> promise;
  };

  void perform();

  bool addContinueReadingHandle(CURL *easyHandle);

  static size_t recvBody(char *buffer, size_t size, size_t nmemb,
                         void *userdata);

  static bool setResponseReady(CurlStorage *curlStorage);

  /**
   * @brief
   * @note This method does not perform any locking.
   */
  void close();

  std::thread performThread_;

  std::atomic<bool> running_ = {false};

  Lock::SpinLock reqLock_;
  std::list<std::unique_ptr<CurlStorage>> reqQueue_;
  std::atomic<size_t> reqQueueSize_ = {0};

  Lock::SpinLock continueReadingLock_;
  std::list<CURL *> continueReadingQueue_;
  std::atomic<size_t> continueReadingQueueSize_ = {0};

  CURLM *mCurl_ = nullptr;

  std::unordered_map<CURL *, std::unique_ptr<CurlStorage>> runningCurl_;

  // todo
  std::condition_variable stopCV_;
  std::atomic<bool> stop_ = {false};
};

} // namespace Http
} // namespace Darabonba