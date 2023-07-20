#ifndef DARABONBA_CORE_H_
#define DARABONBA_CORE_H_

#include <darabonba/JSON.hpp>
#include <darabonba/RuntimeOptions.hpp>
#include <darabonba/http/Request.hpp>
#include <darabonba/http/Response.hpp>
#include <memory>
#include <string>

namespace Darabonba {
class Core {
public:
  static std::shared_ptr<Http::Response>
  doAction(const Http::Request &request, const RuntimeOptions &runtime);
  static std::shared_ptr<Http::Response> doAction(const Http::Request &request);
  static bool allowRetry(const JSON &retry, int retryTimes);
  static int getBackoffTime(const JSON &backoff, int retryTimes);
  static void sleep(int seconds);
  static std::string uuid();
  static JSON merge(const JSON &jf, const JSON &jb);
};

} // namespace Darabonba

#endif