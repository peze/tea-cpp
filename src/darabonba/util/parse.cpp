#include <darabonba/Util.hpp>
#include <darabonba/http/MCurlResponse.hpp>
#include <darabonba/http/Query.hpp>
#include <fstream>
#include <memory>
#include <regex>
#include <sstream>
#include <string>

using std::map;
using std::string;

namespace Darabonba {

std::vector<uint8_t> Util::readAsBytes(std::shared_ptr<Stream> raw) {
  if (raw == nullptr)
    return {};
  auto mCurlRespBody =
      std::dynamic_pointer_cast<Http::MCurlResponse::Body>(raw);
  if (mCurlRespBody) {
    string str = readAsString(mCurlRespBody);
    return std::vector<uint8_t>(str.begin(), str.end());
  }

  // Darabonba::FStream, Darabonba::SStream
  auto p = std::dynamic_pointer_cast<std::basic_iostream<char>>(raw);
  if (p) {
    auto pos = p->tellg();
    p->seekg(0, std::ios::end);
    auto size = p->tellg() - pos;
    p->seekg(pos, std::ios::beg);
    std::vector<uint8_t> ret(size);
    p->read(reinterpret_cast<char *>(&ret[0]), size);
    return ret;
  }
  return {};
}

string Util::readAsString(std::shared_ptr<Stream> raw) {
  if (raw == nullptr)
    return "";
  auto mCurlRespBody =
      std::dynamic_pointer_cast<Http::MCurlResponse::Body>(raw);
  if (mCurlRespBody) {
    string ret;
    while (mCurlRespBody->readFromCurl() > 0) {
      ret += mCurlRespBody->str();
    }
    return ret;
  }
  // Darabonba::FStream, Darabonba::SStream
  auto p = std::dynamic_pointer_cast<std::basic_iostream<char>>(raw);
  if (p) {
    auto pos = p->tellg();
    p->seekg(0, std::ios::end);
    auto size = p->tellg() - pos;
    p->seekg(pos, std::ios::beg);
    string ret;
    ret.resize(size);
    p->read(reinterpret_cast<char *>(&ret[0]), size);
    return ret;
  }
  return "";
}

JSON Util::readAsJSON(std::shared_ptr<Stream> raw) {
  if (raw == nullptr)
    return {};
  auto mCurlRespBody =
      std::dynamic_pointer_cast<Http::MCurlResponse::Body>(raw);
  if (mCurlRespBody) {
    return JSON::parse(readAsString(raw));
  }
  // Darabonba::FStream, Darabonba::SStream
  auto p = std::dynamic_pointer_cast<std::basic_iostream<char>>(raw);
  return JSON::parse(*p);
}

string Util::toFormString(const JSON &val) {
  if (val.empty() || val.is_null()) {
    return "";
  }
  std::stringstream tmp;
  for (const auto &el : val.items()) {
    tmp << Http::Query::encode(el.key()) << " = "
        << Http::Query::encode(el.value().get<string>()) << "&";
  }
  string formstring = tmp.str();
  formstring.pop_back();
  return formstring;
}

JSON Util::anyifyMapValue(const map<string, string> &m) {
  if (m.empty()) {
    return JSON();
  }
  JSON data = m;
  return data;
}

map<string, string> Util::stringifyMapValue(const JSON &m) {
  if (m.empty() || m.is_null()) {
    return {};
  }
  return m.get<map<string, string>>();
}

} // namespace Darabonba