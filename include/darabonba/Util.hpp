#ifndef DARABONBA_UTIL_H_
#define DARABONBA_UTIL_H_

#include <chrono>
#include <cstdint>
#include <darabonba/Core.hpp>
#include <darabonba/Exception.hpp>
#include <darabonba/Stream.hpp>
#include <darabonba/http/Response.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <thread>
#include <vector>

namespace Darabonba {
class Util {
public:
  /************************** helper **************************/
  static std::string getNonce() { return Core::uuid(); }

  static std::string getDateUTCString();

  static std::string getUserAgent(const std::string &userAgent);

  static void sleep(int millisecond) {
    std::this_thread::sleep_for(std::chrono::milliseconds(millisecond));
  }

  static void validateModel(const std::shared_ptr<Model> &m) { m->validate(); }

  /************************** default **************************/
  static std::string defaultString(const std::string &real,
                                   const std::string &defaultVal) {
    return real.empty() ? defaultVal : real;
  }

  static int64_t defaultNumber(int64_t real, int64_t defaultVal) {
    return real ? real : defaultVal;
  }

  /************************** parse **************************/
  static std::map<std::string, std::string> stringifyMapValue(const JSON &m);

  static JSON anyifyMapValue(const std::map<std::string, std::string> &m);

  static JSON toMap(const std::shared_ptr<Model> &in) {
    if (!in) {
      return JSON();
    }
    return in->toMap();
  }

  static std::vector<uint8_t> toBytes(const std::string &val) {
    return std::vector<uint8_t>(val.begin(), val.end());
  }

  static std::string toString(const std::vector<uint8_t> &val) {
    return std::string(val.begin(), val.end());
  }

  static std::vector<JSON> toArray(const JSON &input) {
    if (!input.is_array()) {
      throw Exception("input is not a array");
    }
    return input.get<std::vector<JSON>>();
  };

  static std::string toFormString(const JSON &val);

  static std::string toJSONString(const JSON &value) { return value.dump(); }

  static JSON parseJSON(const std::string &val) { return JSON::parse(val); }

  static std::vector<uint8_t> readAsBytes(std::shared_ptr<Stream> raw);

  static std::string readAsString(std::shared_ptr<Stream> raw);

  static JSON readAsJSON(std::shared_ptr<Stream> raw);

  /************************** assert **************************/
  static bool isUnset(const JSON &value) {
    return value.empty() || value.is_null();
  };

  static bool empty(const std::string &val) { return val.empty(); }

  static bool equalString(const std::string &val1, const std::string &val2) {
    return val1 == val2;
  }

  static bool equalNumber(int64_t val1, int64_t val2) { return val1 == val2; }

  static bool assertAsBoolean(const JSON &value);

  static std::string assertAsString(const JSON &value);

  static std::vector<uint8_t> assertAsBytes(const JSON &value);

  static int64_t assertAsNumber(const JSON &value);

  static JSON assertAsMap(const JSON &value);

  // todo
  static Stream *assertAsReadable(void *value);

  static bool is2xx(const int64_t &code) { return 200 <= code && code < 300; }

  static bool is3xx(const int64_t &code) { return 300 <= code && code < 400; }

  static bool is4xx(const int64_t &code) { return 400 <= code && code < 500; }

  static bool is5xx(const int64_t &code) { return 500 <= code && code < 600; }
};
} // namespace Darabonba

#endif
