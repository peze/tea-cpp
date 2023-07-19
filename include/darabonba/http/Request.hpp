#ifndef DARABONBA_HTTP_REQUEST_H_
#define DARABONBA_HTTP_REQUEST_H_

#include <darabonba/Stream.hpp>
#include <darabonba/http/Header.hpp>
#include <darabonba/http/URL.hpp>
#include <iostream>
#include <memory>
#include <string>

namespace Darabonba {
namespace Http {

class Request {
public:
  enum class Method {
    GET,
    POST,
    HEAD,
    PUT,
    DELETE,
    TRACE,
    CONNECT,
    OPTIONS,
    PATCH,
  };

  Request() = default;
  Request(const std::string &url) : url_(url) {}
  Request(const URL &url) : url_(url) {}
  ~Request() = default;

  const URL &url() const { return url_; }
  URL &url() { return url_; }

  Method method() const { return method_; };
  std::string strMethod() const;
  void setMethod(Method method) { method_ = method; };

  const Query &query() const { return url_.query(); }
  Query &query() { return url_.query(); }

  std::string header(const std::string key) const {
    auto it = header_.find(key);
    if (it != header_.end())
      return it->second;
    return "";
  }
  const Header &header() const { return header_; };
  Header &header() { return header_; }

  std::shared_ptr<IStream> body() const { return body_; }
  void setBody(std::shared_ptr<IStream> body) { body_ = body; }

protected:
  URL url_;
  Method method_ = Method::GET;
  Header header_;
  std::shared_ptr<IStream> body_;
  // stringstream, fstream
};

} // namespace Http
} // namespace Darabonba

#endif