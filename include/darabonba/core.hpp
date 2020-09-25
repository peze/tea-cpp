#ifndef DARABONBA_CORE_H_
#define DARABONBA_CORE_H_

#include <boost/any.hpp>
#include <boost/exception/all.hpp>
#include <cpprest/streams.h>
#include <iostream>

using namespace std;

namespace Darabonba {
class Model {
public:
  Model() = default;
  ~Model() = default;
  Model(const map<string, boost::any> &config) {
      _config = config;
  };
  static void validateRequired(const string &field_name,
                               const shared_ptr<boost::any>& field);
  static void validateMaxLength(const string &field_name,
                                const shared_ptr<string>& field,
                                int val);
  static void validateMinLength(const string &field_name,
                                const shared_ptr<string>& field,
                                int val);
  static void validateMaximum(const string &field_name,
                              shared_ptr<boost::any> field,
                              boost::any val);
  static void validateMinimum(const string &field_name,
                              shared_ptr<boost::any> field,
                              boost::any val);
  static void validatePattern(const string &field_name,
                              const shared_ptr<string>& field,
                              const string &val);
  virtual void validate(){};

  virtual map<string, boost::any> toMap() = 0;
  virtual void fromMap(map<string, boost::any> m) = 0;

protected:
  map<string, string> _name;
  map<string, boost::any> _default;
  map<string, boost::any> _config;
};
class Request {
public:
  Request();
  ~Request();

  string protocol = "https";
  string host;
  int port = 80;
  string method = "GET";
  string pathname;
  map<string, string> query;
  string body;
  map<string, string> headers;
};
class Response {
public:
  Response();
  ~Response();

  concurrency::streams::istream body;
  int statusCode;
};
class Core {
public:
  static Response doAction(const Request &req,
                            map<string, boost::any> runtime);
  static Response doAction(const Request &req);
  static bool allowRetry(const shared_ptr<boost::any>& retry,
                         const shared_ptr<int>& retry_times,
                         const shared_ptr<int>& now);
  static int getBackoffTime(const shared_ptr<boost::any>& backoff,
                            const shared_ptr<int>& retry_times);
  static void sleep(const shared_ptr<int>& sleep_time);
  static bool isRetryable(const exception& ex);
  static bool isRetryable(const boost::exception& ex);
};
class Converter {
public:
  static map<string, string> *mapPointer(map<string, string> m) {
    auto *_m = new map<string, string>();
    if (m.empty()) {
      return _m;
    }
    for (auto it : m) {
      _m->insert(pair<string, string>(it.first, it.second));
    }
    return _m;
  }

  static map<string, boost::any> *mapPointer(map<string, boost::any> m) {
    auto *_m = new map<string, boost::any>();
    if (m.empty()) {
      return _m;
    }
    for (auto it : m) {
      _m->insert(pair<string, boost::any>(it.first, it.second));
    }
    return _m;
  }

  static string toString(boost::any val) {
    if (typeid(string) == val.type()) {
      return boost::any_cast<string>(val);
    }
    return string("");
  }

  template <typename... Params>
  static map<string, string> merge(map<string, string> m,
                                   Params... parameters) {
    mergeMap(m, parameters...);
    return m;
  }

  template <typename... Params>
  static map<string, boost::any> merge(map<string, boost::any> m,
                                       Params... parameters) {
    mergeMap(m, parameters...);
    return m;
  }
private:
  template <typename... Params>
  static void mergeMap(map<string, string>& m,
                       map<string, string> value,
                       Params... parameters) {
    assignMap(m, value);
    mergeMap(m, parameters...);
  }
  static void mergeMap(map<string, string>& m,
                       map<string, string> value) {
    assignMap(m, value);
  }

  static void assignMap(map<string, string>& m,
                        map<string, string> value) {
    for (auto it : value) {
      m.insert(pair<string, string>(it.first, it.second));
    }
  }

  template <typename... Params>
  static void mergeMap(map<string, boost::any>& m,
                       map<string, boost::any> value,
                       Params... parameters) {
    assignMap(m, value);
    mergeMap(m, parameters...);
  }
  static void mergeMap(map<string, boost::any>& m,
                       map<string, boost::any> value) {
    assignMap(m, value);
  }

  static void assignMap(map<string, boost::any>& m,
                        map<string, boost::any> value) {
    for (auto it : value) {
      m.insert(pair<string, boost::any>(it.first, it.second));
    }
  }
};
struct Error : virtual exception, virtual boost::exception {
public:
  Error();
  Error(map<string, string> error_info);
  Error(map<string, boost::any> error_info);
  string message;
  string code;
  string data;
  string name;
};
struct UnretryableError : public Error {
public:
  UnretryableError(const Request &last_request,
                   const std::exception &last_exception) {
    _last_request = last_request;
    _last_exception = last_exception;
  }
  Darabonba::Request getLastRequest();
  std::exception getLastException();

private:
  Request _last_request;
  std::exception _last_exception;
};
} // namespace Darabonba

#endif