#include <darabonba/Core.hpp>

#include <chrono>
#include <darabonba/http/MCurlHttpClient.hpp>
#include <fstream>
#include <sstream>
#include <thread>

#ifdef _WIN32
#include <Windows.h>
#include <objbase.h>
#else
#include <uuid/uuid.h>
#endif

using std::string;
namespace Darabonba {

struct Deleter {
  void operator()(Http::MCurlHttpClient *ptr) {
    delete ptr;
    curl_global_cleanup();
  }
};

void Core::sleep(int seconds) {
  std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

string Core::uuid() {
#ifdef _WIN32
  GUID guid;
  if (CoCreateGuid(&guid)) {
    return "";
  }

  std::stringstream str;
  str.setf(std::ios_base::uppercase);
  str << std::hex << std::setfil('0') << std::setw(8) << guid.Data1 << '-'
      << std::setw(4) << guid.Data2 << '-' << std::setw(4) << guid.Data3 << '-'
      << std::setw(2) << (int)guid.Data4[0] << std::setw(2)
      << (int)guid.Data4[1] << '-' << std::setw(2) << (int)guid.Data4[2]
      << std::setw(2) << (int)guid.Data4[3] << '-' << std::setw(2)
      << (int)guid.Data4[4] << std::setw(2) << (int)guid.Data4[5]
      << std::setw(2) << (int)guid.Data4[6] << std::setw(2)
      << (int)guid.Data4[7];

  return str.str();
#else
  std::stringstream str;
  str.setf(std::ios_base::uppercase);
  uuid_t uu;
  uuid_generate(uu);

  str << std::hex << std::setfill('0') << std::setw(2) << (int)uu[0]
      << std::setw(2) << (int)uu[1] << std::setw(2) << (int)uu[2]
      << std::setw(2) << (int)uu[3] << "-" << std::setw(2) << (int)uu[4]
      << std::setw(2) << (int)uu[5] << "-" << std::setw(2) << (int)uu[6]
      << std::setw(2) << (int)uu[7] << "-" << std::setw(2) << (int)uu[8]
      << std::setw(2) << (int)uu[9] << "-" << std::setw(2) << (int)uu[10]
      << std::setw(2) << (int)uu[11] << std::setw(2) << (int)uu[12]
      << std::setw(2) << (int)uu[13] << std::setw(2) << (int)uu[14]
      << std::setw(2) << (int)uu[15];
  return str.str();
#endif
}

int Core::getBackoffTime(const JSON &backoff, int retryTimes) {
  if (backoff.empty()) {
    return 0;
  }
  int rt = !retryTimes ? 0 : retryTimes;
  int backOffTime = 0;
  string policy = "no";
  if (backoff.find("policy") != backoff.end()) {
    policy = backoff["policy"].get<string>();
  }
  if (policy == "no") {
    return backOffTime;
  }

  if (backoff.find("period") != backoff.end()) {
    int period = backoff["period"].get<int>();
    backOffTime = period;
    if (backOffTime <= 0) {
      return rt;
    }
  }
  return backOffTime;
}

bool Core::allowRetry(const JSON &retry, int retryTimes) {
  if (retry.empty()) {
    return false;
  }
  int maxAttempts = 0;
  if (retry.find("maxAttempts") != retry.end()) {
    maxAttempts = retry["maxAttempts"].get<int>();
    return maxAttempts >= retryTimes;
  }
  return false;
}

JSON Core::merge(const JSON &jf, const JSON &jb) {
  JSON ret;
  ret.merge_patch(jf);
  ret.merge_patch(jb);
  return ret;
}

std::future<std::shared_ptr<Http::Response>>
Core::doAction(const Http::Request &request, const RuntimeOptions &runtime) {
  static auto client = []() {
    curl_global_init(CURL_GLOBAL_ALL);
    auto ret = std::unique_ptr<Http::MCurlHttpClient, Deleter>(
        new Http::MCurlHttpClient());
    ret->start();
    return ret;
  }();
  return client->makeRequest(request, runtime);
}

} // namespace Darabonba