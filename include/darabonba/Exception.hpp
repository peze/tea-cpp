#ifndef DARABONBA_CORE_ERROR_H_
#define DARABONBA_CORE_ERROR_H_

#include <darabonba/JSON.hpp>
#include <darabonba/Model.hpp>
#include <darabonba/http/Request.hpp>
#include <string>

namespace Darabonba {

class Exception : public std::exception {

  friend void to_json(JSON &j, const Exception &obj) {
    DARABONBA_TO_JSON(code, code_);
    DARABONBA_TO_JSON(message, message_);
    DARABONBA_TO_JSON(data, data_);
    DARABONBA_TO_JSON(description, description_);
    DARABONBA_TO_JSON(accessDeniedDetail, accessDeniedDetail_);
    // DARABONBA_TO_JSON(statusCode, statusCode_);
  }
  friend void from_json(const JSON &j, Exception &obj) {
    DARABONBA_FROM_JSON(code, code_, std::string);
    DARABONBA_FROM_JSON(message, message_, std::string);
    DARABONBA_FROM_JSON(data, data_, JSON);
    DARABONBA_FROM_JSON(description, description_, std::string);
    DARABONBA_FROM_JSON(accessDeniedDetail, accessDeniedDetail_, JSON);
    if (j.count("data") && j["data"].count("statusCode")) {
      obj.statusCode_ = j["data"]["statusCode"];
    }
  }

public:
  Exception() = default;
  Exception(const std::string &msg) : message_(msg) {}
  Exception(const char *msg) : message_(msg) {}
  Exception(const JSON &errorInfo) { from_json(errorInfo, *this); }

  const char *what() const noexcept override { return message_.c_str(); }

  const std::string &code() const { return code_; }
  void setCode(const std::string &code) { code_ = code; }

  const std::string &message() const { return message_; }
  void setMessage(const std::string &message) { message_ = message; }

  const JSON &data() const { return data_; }
  void setData(const JSON &data) { data_ = data; }

  int statusCode() const { return statusCode_; }
  void setStatusCode(int statusCode) { statusCode_ = statusCode; }

  const std::string &description() const { return description_; }
  void setDescription(const std::string &description) {
    description_ = description;
  }

  const JSON &accessDeniedDetail() const { return accessDeniedDetail_; }
  void setAccessDeniedDetail(const JSON &accessDeniedDetail) {
    accessDeniedDetail_ = accessDeniedDetail;
  }

protected:
  std::string code_;
  std::string message_;
  JSON data_;
  int statusCode_;
  std::string description_;
  JSON accessDeniedDetail_;
};

class UnretryableException : public Exception {
public:
  UnretryableException() = default;
  UnretryableException(const Http::Request &lastRequest,
                       const JSON &lastException)
      : lastRequest_(lastRequest), lastException_(lastException) {}
  UnretryableException(const Http::Request &lastRequest)
      : lastRequest_(lastRequest) {}
  UnretryableException(const JSON &lastException)
      : lastException_(lastException) {}

  const Http::Request &getLastRequest() const { return lastRequest_; }
  const Exception &getLastException() const { return lastException_; }

protected:
  Http::Request lastRequest_;
  Exception lastException_;
};

class RetryableException : public Exception {
public:
  RetryableException() = default;
  RetryableException(const JSON &errorInfo) : Exception(errorInfo){};
};

class ValidateException : public Exception {
public:
  ValidateException() = default;
  ValidateException(const JSON &errorInfo) : Exception(errorInfo){};
};

} // namespace Darabonba
#endif