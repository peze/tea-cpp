#ifndef DARABONBA_CORE_H_
#define DARABONBA_CORE_H_

#include <darabonba/Type.hpp>
#include <darabonba/RuntimeOptions.hpp>
#include <darabonba/http/Request.hpp>
#include <darabonba/http/Response.hpp>
#include <future>
#include <memory>
#include <string>

namespace Darabonba {
class Core {
public:
  static std::future<std::shared_ptr<Http::Response>>
  doAction(const Http::Request &request,
           const RuntimeOptions &runtime = RuntimeOptions());
  static bool allowRetry(const Json &retry, int retryTimes);
  static int getBackoffTime(const Json &backoff, int retryTimes);
  static void sleep(int seconds);
  static std::string uuid();
  static Json merge(const Json &jf, const Json &jb);
};

} // namespace Darabonba

#endif