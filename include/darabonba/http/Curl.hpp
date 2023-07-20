#ifndef DARABONBA_HTTP_CURL_H_
#define DARABONBA_HTTP_CURL_H_

#include <curl/curl.h>
#include <darabonba/http/Header.hpp>
#include <memory>

namespace Darabonba {

namespace Http {

namespace Curl {
struct CURLMDeleter {
  void operator()(CURLM *ptr) { curl_multi_cleanup(ptr); }
};

struct CURLDeleter {
  void operator()(CURL *ptr) { curl_easy_cleanup(ptr); }
};

struct CURLMDeleterWithGlobal {
  void operator()(CURLM *ptr) {
    curl_multi_cleanup(ptr);
    curl_global_cleanup();
  }
};

/**
 * @brief A callback function that read data from std::istringstream.
 */
size_t readISStream(char *buffer, size_t size, size_t nitems, void *userdata);
/**
 * @brief A callback function that read data from std::ifstream
 */
size_t readIFStream(char *buffer, size_t size, size_t nitems, void *userdata);

/**
 * @note The header callback will be called once for each header and only
 * complete header lines are passed on to the callback. Do not assume that the
 * header line is null-terminated!
 * @ref https://curl.se/libcurl/c/CURLOPT_HEADERFUNCTION.html
 */
size_t writeHeader(char *buffer, size_t size, size_t nitems, void *userdata);

size_t writeOSStream(char *buffer, size_t size, size_t nitems, void *userdata);

size_t writeOFStream(char *buffer, size_t size, size_t nitems, void *userdata);

size_t recvBody(void *ptr, size_t size, size_t nmemb, void *userdata);

void setCurlRequestBody(CURL *curl, std::shared_ptr<IStream> body);

void setCurlProxy(CURL *curl, const std::string &proxy);

curl_slist *setCurlHeader(CURL *curl, const Darabonba::Http::Header &header);

} // namespace Curl
} // namespace Http
} // namespace Darabonba

#endif