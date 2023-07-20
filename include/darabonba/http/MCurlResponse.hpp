#ifndef DARABONBA_HTTP_MCURLRESPINSE_H_
#define DARABONBA_HTTP_MCURLRESPINSE_H_

#include <darabonba/Stream.hpp>
#include <darabonba/http/Response.hpp>

namespace Darabonba {
namespace Http {
class MCurlResponse : public Response {
public:
  class Body : public OStream, public std::stringstream {
  public:
    Body(CURLM *mcurl, CURL *curl) : mcurl_(mcurl), curl_(curl) {}
    virtual ~Body() {
      curl_multi_remove_handle(mcurl_, curl_);
      curl_easy_cleanup(curl_);
    }
    size_t readFromCurl();
    void write(char *ptr, size_t size);
    bool ready() const { return ready_; }
    void setReady(bool ready) { ready_ = ready; }
    bool flow() const { return flow_; }
    void setFlow(bool flow) { flow_ = flow; }

  protected:
    enum { MAX_SIZE = 6000 };
    CURLM *mcurl_;
    CURL *curl_;

    size_t size_;
    bool flow_ = false;
    bool ready_ = false;
  };

  MCurlResponse(CURLM *mcurl, CURL *curl) : curl_(curl) {
    // todo: new body
  }
  virtual int statusCode() const {
    if (statusCode_ == UNINITIALIZED_STATUS_CODE) {
      curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &statusCode_);
    }
    return statusCode_;
  }

protected:
  CURL *curl_;
};
} // namespace Http

} // namespace Darabonba

#endif