#include <darabonba/Stream.hpp>
#include <darabonba/http/Curl.hpp>
#include <darabonba/http/FileField.hpp>
#include <darabonba/http/MCurlResponse.hpp>
#include <darabonba/http/URL.hpp>
#include <memory>

namespace Darabonba {
namespace Http {
namespace Curl {

size_t readISStream(char *buffer, size_t size, size_t nitems, void *userdata) {
  auto ss = static_cast<ISStream *>(userdata);
  if (ss == nullptr || ss->eof() || ss->bad())
    return 0;
  auto expectSize = size * nitems;
  return ss->readsome(buffer, expectSize);
}

size_t readIFStream(char *buffer, size_t size, size_t nitems, void *userdata) {
  auto ss = static_cast<IFStream *>(userdata);
  if (ss == nullptr || ss->eof() || ss->bad())
    return 0;
  auto expectSize = size * nitems;
  auto realSize = ss->readsome(buffer, expectSize);
  if (realSize < 0)
    return 0;
  else if (realSize == 0) {
    ss->read(buffer, size);
    return ss->gcount();
  }
  return realSize;
}

size_t readCustomIStream(char *buffer, size_t size, size_t nitems,
                         void *userdata) {
  auto ss = static_cast<CustomIStream *>(userdata);
  return ss->write(buffer, size * nitems);
}

size_t writeHeader(char *buffer, size_t size, size_t nitems, void *userdata) {
  auto header = static_cast<Header *>(userdata);
  std::string s(buffer, size * nitems);
  auto pos = s.find(':');
  if (pos == std::string::npos) {
    // todo: handle the error
    return size * nitems;
  }
  header->emplace(s.substr(0, pos), s.substr(pos + 2));
  return size * nitems;
}

size_t writeOSStream(char *buffer, size_t size, size_t nitems, void *userdata) {
  auto ss = static_cast<OSStream *>(userdata);
  if (ss == nullptr || ss->bad())
    return 0;
  ss->write(buffer, size * nitems);
  return size * nitems;
}

size_t writeOFStream(char *buffer, size_t size, size_t nitems, void *userdata) {
  auto ss = static_cast<OFStream *>(userdata);
  if (ss == nullptr || ss->bad())
    return 0;
  ss->write(buffer, size * nitems);
  return size * nitems;
}

size_t recvBody(char *buffer, size_t size, size_t nmemb, void *userdata) {
  auto *out = static_cast<MCurlResponse::Body *>(userdata);
  if (!out->ready()) {
    out->setReady(true);
  }
  bool flow = out->flow();
  if (!flow) {
    return CURL_WRITEFUNC_PAUSE;
  }
  size_t bufferSize = size * nmemb;
  out->write(buffer, bufferSize);
  return bufferSize;
}

void setCurlRequestBody(CURL *curl, std::shared_ptr<IStream> body) {
  auto ss = std::dynamic_pointer_cast<ISStream>(body);
  if (ss) {
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, readISStream);
    return;
  }
  auto fs = std::dynamic_pointer_cast<IFStream>(body);
  if (fs) {
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, readIFStream);
    return;
  }
  auto cs = std::dynamic_pointer_cast<CustomIStream>(body);
  if (cs) {
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, readCustomIStream);
    return;
  }
  auto ffs = std::dynamic_pointer_cast<FileFormStream>(body);
  if (ffs) {
    ffs->setMine(curl_mime_init(curl));
    auto mime = ffs->mime();
    for (const auto &fileform : *ffs) {
      auto part = curl_mime_addpart(mime);
      curl_mime_name(part, fileform.filename().c_str());
      curl_mime_filedata(part, fileform.content().c_str());
      curl_mime_type(part, fileform.contentType().c_str());
    }
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
    return;
  }
}

void setCurlProxy(CURL *curl, const std::string &proxy) {
  URL url(proxy);
  // todo
  std::ostringstream out;
  out << url.host() << ":" << url.port();
  curl_easy_setopt(curl, CURLOPT_PROXY, out.str().c_str());
  if (!url.userInfo().empty()) {
    curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, url.userInfo().c_str());
  }
}

curl_slist *setCurlHeader(CURL *curl, const Darabonba::Http::Header &header) {
  curl_slist *list = nullptr;
  for (const auto &p : header) {
    list = curl_slist_append(list, (p.first + ": " + p.second).c_str());
  }
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
  return list;
}
} // namespace Curl
} // namespace Http
} // namespace Darabonba