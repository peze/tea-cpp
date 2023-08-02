#ifndef DARABONBA_HTTP_RESPONSE_H_
#define DARABONBA_HTTP_RESPONSE_H_
#include <curl/curl.h>
#include <darabonba/Stream.hpp>
#include <darabonba/http/Curl.hpp>
#include <darabonba/http/Header.hpp>
#include <darabonba/http/ResponseBase.hpp>
#include <map>
#include <memory>
#include <sstream>
#include <string>

namespace Darabonba {
namespace Http {

class Response : public ResponseBase {
public:
  Response() = default;
  virtual ~Response() = default;

  virtual std::shared_ptr<IOStream> body() const { return body_; };

  virtual void setBody(std::shared_ptr<IOStream> body) { body_ = body; }

protected:
  std::shared_ptr<IOStream> body_;
};

} // namespace Http
} // namespace Darabonba
#endif