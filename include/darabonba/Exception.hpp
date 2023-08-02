#ifndef DARABONBA_CORE_ERROR_H_
#define DARABONBA_CORE_ERROR_H_

#include <darabonba/Model.hpp>
#include <darabonba/Type.hpp>
#include <darabonba/http/Request.hpp>
#include <string>

namespace Darabonba {

class Exception : public std::exception {

  friend void to_json(Json &j, const Exception &obj) {
    DARABONBA_TO_JSON(code, code_);
    DARABONBA_TO_JSON(message, message_);
    DARABONBA_TO_JSON(data, data_);
    DARABONBA_TO_JSON(description, description_);
    DARABONBA_TO_JSON(accessDeniedDetail, accessDeniedDetail_);
    // DARABONBA_TO_JSON(statusCode, statusCode_);
  }
  friend void from_json(const Json &j, Exception &obj) {
    DARABONBA_FROM_JSON(code, code_);
    DARABONBA_FROM_JSON(message, message_);
    DARABONBA_FROM_JSON(data, data_);
    DARABONBA_FROM_JSON(description, description_);
    DARABONBA_FROM_JSON(accessDeniedDetail, accessDeniedDetail_);
    if (j.count("data") && j["data"].count("statusCode")) {
      obj.statusCode_ = j["data"]["statusCode"];
    }
  }

public:
  Exception() = default;
  Exception(const std::string &msg) : message_(msg) {}
  Exception(const char *msg) : message_(msg) {}
  Exception(const Json &errorInfo) { from_json(errorInfo, *this); }

  const char *what() const noexcept override { return message_.c_str(); }

  const std::string &code() const { return code_; }
  void setCode(const std::string &code) { code_ = code; }

  const std::string &message() const { return message_; }
  void setMessage(const std::string &message) { message_ = message; }

  const Json &data() const { return data_; }
  void setData(const Json &data) { data_ = data; }

  int statusCode() const { return statusCode_; }
  void setStatusCode(int statusCode) { statusCode_ = statusCode; }

  const std::string &description() const { return description_; }
  void setDescription(const std::string &description) {
    description_ = description;
  }

  const Json &accessDeniedDetail() const { return accessDeniedDetail_; }
  void setAccessDeniedDetail(const Json &accessDeniedDetail) {
    accessDeniedDetail_ = accessDeniedDetail;
  }

protected:
  std::string code_;
  std::string message_;
  Json data_;
  // todo
  int statusCode_;
  std::string description_;
  Json accessDeniedDetail_;
};

class UnretryableException : public Exception {
public:
  UnretryableException() = default;
  UnretryableException(const Http::Request &lastRequest,
                       const Json &lastException)
      : lastRequest_(lastRequest), lastException_(lastException) {}
  UnretryableException(const Http::Request &lastRequest)
      : lastRequest_(lastRequest) {}
  UnretryableException(const Json &lastException)
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
  RetryableException(const Json &errorInfo) : Exception(errorInfo){};
};

class ValidateException : public Exception {
public:
  ValidateException() = default;
  ValidateException(const Json &errorInfo) : Exception(errorInfo){};
};

} // namespace Darabonba
#endif