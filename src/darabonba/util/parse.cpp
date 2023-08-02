#include <darabonba/Util.hpp>
#include <darabonba/http/MCurlResponseBody.hpp>
#include <darabonba/http/Query.hpp>
#include <fstream>
#include <memory>
#include <regex>
#include <sstream>
#include <string>

using std::map;
using std::string;

namespace Darabonba {

Bytes Util::readAsBytes(std::shared_ptr<IStream> raw) {
  if (raw == nullptr)
    return {};
  // Darabonba::IFStream, Darabonba::ISStream
  auto basicIStream = std::dynamic_pointer_cast<std::basic_istream<char>>(raw);
  if (basicIStream) {
    auto pos = basicIStream->tellg();
    basicIStream->seekg(0, std::ios::end);
    auto size = basicIStream->tellg() - pos;
    basicIStream->seekg(pos, std::ios::beg);
    Bytes ret(size);
    basicIStream->read(reinterpret_cast<char *>(&ret[0]), size);
    return ret;
  }
  // Http::MCurlResponseBody
  auto respBody = std::dynamic_pointer_cast<Http::MCurlResponseBody>(raw);
  if (respBody) {
    respBody->waitForDone();
    auto size = respBody->readableSize();
    Bytes ret(size);
    respBody->read(reinterpret_cast<char *>(&ret[0]), size);
    return ret;
  }
  // todo custom IStream
  return {};
}

string Util::readAsString(std::shared_ptr<IStream> raw) {
  if (raw == nullptr)
    return "";
  // Darabonba::IFStream, Darabonba::ISStream
  auto basicIStream = std::dynamic_pointer_cast<std::basic_istream<char>>(raw);
  if (basicIStream) {
    auto pos = basicIStream->tellg();
    basicIStream->seekg(0, std::ios::end);
    auto size = basicIStream->tellg() - pos;
    basicIStream->seekg(pos, std::ios::beg);
    string ret;
    ret.resize(size);
    basicIStream->read(reinterpret_cast<char *>(&ret[0]), size);
    return ret;
  }
  // Http::MCurlResponseBody
  auto respBody = std::dynamic_pointer_cast<Http::MCurlResponseBody>(raw);
  if (respBody) {
    respBody->waitForDone();
    auto size = respBody->readableSize();
    string ret;
    ret.resize(respBody->readableSize());
    respBody->read(reinterpret_cast<char *>(&ret[0]), size);
    return ret;
  }
  // todo custom IStream
  return "";
}

Json Util::readAsJSON(std::shared_ptr<IStream> raw) {
  if (raw == nullptr)
    return {};
  // Darabonba::IFStream, Darabonba::ISStream
  auto basicIStream = std::dynamic_pointer_cast<std::basic_istream<char>>(raw);
  if (basicIStream) {
    return Json::parse(*basicIStream);
  }
  auto str = readAsString(raw);
  return Json::parse(str);
}

string Util::toFormString(const Json &val) {
  if (val.empty() || val.is_null()) {
    return "";
  }
  std::stringstream tmp;
  for (const auto &el : val.items()) {
    tmp << Http::Query::encode(el.key()) << " = "
        << Http::Query::encode(el.value().is_string()
                                   ? el.value().get<std::string>()
                                   : el.value().dump())
        << "&";
  }
  string formstring = tmp.str();
  formstring.pop_back();
  return formstring;
}

} // namespace Darabonba