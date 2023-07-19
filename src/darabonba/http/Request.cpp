#include <darabonba/http/Request.hpp>

namespace Darabonba {
namespace Http {
std::string Request::strMethod() const {
  switch (method_) {
  case Method::GET:
    return "GET";
  case Method::POST:
    return "POST";
  case Method::HEAD:
    return "HEAD";
  case Method::PUT:
    return "PUT";
  case Method::DELETE:
    return "DELETE";
  case Method::TRACE:
    return "TRACE";
  case Method::CONNECT:
    return "CONNECT";
  case Method::OPTIONS:
    return "OPTIONS";
  case Method::PATCH:
    return "PATCH";
  }
}
} // namespace Http
} // namespace Darabonba