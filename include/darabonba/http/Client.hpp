
#ifndef DARABONBA_HTTP_CLIENT_H_
#define DARABONBA_HTTP_CLIENT_H_
#include <curl/curl.h>
#include <darabonba/RuntimeOptions.hpp>
#include <darabonba/http/Curl.hpp>
#include <memory>

namespace Darabonba {
namespace Http {
class Client {
public:
  Client(CURLM *mcurl = nullptr) {
    mCurlHandle_ = (mcurl ? mcurl : getDefaultMCULM());
  }
  ~Client() = default;

  std::shared_ptr<Response> doAction(const Request &request,
                                     const RuntimeOptions &runtimeOptions);

  static CURLM *getDefaultMCULM() { return defaultMCurl_.get(); }

protected:
  CURLM *mCurlHandle_ = nullptr;
  static std::unique_ptr<CURLM, Curl::CURLMDeleterWithGlobal> defaultMCurl_;
};
} // namespace Http
} // namespace Darabonba

#endif