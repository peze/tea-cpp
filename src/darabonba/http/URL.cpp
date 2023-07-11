
#include <algorithm>
#include <cctype>
#include <curl/curl.h>
#include <darabonba/http/URL.hpp>
#include <iomanip>

namespace Darabonba {
namespace Http {

URL::URL(const std::string &url) {
  CURLU *curlu = curl_url();
  if (curlu == nullptr)
    return;
  curl_url_set(curlu, CURLUPART_URL, url.c_str(), 0);
  char *scheme = nullptr, *user = nullptr, *password, *host = nullptr,
       *port = nullptr, *path = nullptr, *query = nullptr, *fragment = nullptr;
  curl_url_get(curlu, CURLUPART_SCHEME, &scheme, 0);
  curl_url_get(curlu, CURLUPART_USER, &user, 0);
  curl_url_get(curlu, CURLUPART_USER, &password, 0);
  curl_url_get(curlu, CURLUPART_HOST, &host, 0);
  curl_url_get(curlu, CURLUPART_PATH, &path, 0);
  curl_url_get(curlu, CURLUPART_PORT, &port, 0);
  curl_url_get(curlu, CURLUPART_QUERY, &query, 0);
  curl_url_get(curlu, CURLUPART_FRAGMENT, &fragment, 0);

  scheme_ = scheme;
  user_ = user;
  password_ = password;
  host_ = host;
  path_ = path;
  port_ = std::stoi(port);
  query_ = Query(query);
  fragment_ = fragment;

  curl_free(scheme);
  curl_free(user);
  curl_free(password);
  curl_free(host);
  curl_free(path);
  curl_free(port);
  curl_free(query);
  curl_free(fragment);
  curl_url_cleanup(curlu);
}

URL::URL::operator std::string() const {
  CURLU *url = curl_url();
  if (url == nullptr)
    return "";
  curl_url_set(url, CURLUPART_SCHEME, scheme_.c_str(), 0);
  curl_url_set(url, CURLUPART_USER, user_.c_str(), 0);
  curl_url_set(url, CURLUPART_PASSWORD, password_.c_str(), 0);
  curl_url_set(url, CURLUPART_HOST, host_.c_str(), 0);
  curl_url_set(url, CURLUPART_PATH, path_.c_str(), 0);
  curl_url_set(url, CURLUPART_PORT, std::to_string(port_).c_str(), 0);
  curl_url_set(url, CURLUPART_QUERY, static_cast<std::string>(query_).c_str(),
               0);
  curl_url_set(url, CURLUPART_FRAGMENT, fragment_.c_str(), 0);

  char *s = nullptr;
  curl_url_get(url, CURLUPART_URL, &s, 0);
  std::string ret = s;
  curl_free(s);
  curl_url_cleanup(url);
  return ret;
}

bool URL::operator==(const URL &url) const {
  return scheme_ == url.scheme_ && user_ == url.user_ &&
         password_ == url.password_ && host_ == url.host_ &&
         path_ == url.path_ && port_ == url.port_ && query_ == url.query_ &&
         fragment_ == url.fragment_;
}

void URL::clear() {
  scheme_.clear();
  user_.clear();
  password_.clear();
  host_.clear();
  path_.clear();
  port_ = INVALID_PORT;
  query_.clear();
  fragment_.clear();
}

bool URL::isEmpty() const {
  return scheme_.empty() && user_.empty() && password_.empty() &&
         host_.empty() && path_.empty() && (port_ == INVALID_PORT) &&
         query_.empty() && fragment_.empty();
}

bool URL::isValid() const {
  if (isEmpty())
    return false;

  if (host_.empty())
    return false;

  bool valid = true;
  if (user_.empty())
    valid = password_.empty();
  return valid;
}

std::string URL::authority() const {
  if (!isValid())
    return "";

  std::ostringstream out;
  std::string str = userInfo();
  if (!str.empty())
    out << str << "@";
  out << host_;
  if (port_ != INVALID_PORT)
    out << ":" << port_;
  return out.str();
}

void URL::setAuthority(const std::string &authority) {
  if (authority.empty()) {
    setUserInfo("");
    setHost("");
    setPort(INVALID_PORT);
    return;
  }

  std::string userinfo, host, port;
  std::string::size_type pos = 0, prevpos = 0;

  pos = authority.find('@');
  if (pos != authority.npos) {
    userinfo = authority.substr(0, pos);
    prevpos = pos + 1;
  } else {
    pos = 0;
  }

  pos = authority.find(':', prevpos);
  if (pos == authority.npos) {
    host = authority.substr(prevpos);
  } else {
    host = authority.substr(prevpos, pos - prevpos);
    port = authority.substr(pos + 1);
  }

  setUserInfo(userinfo);
  setHost(host);
  setPort(!port.empty() ? atoi(port.c_str()) : INVALID_PORT);
}

void URL::setHost(const std::string &host) {
  host_ = host;
  std::transform(host_.begin(), host_.end(), host_.begin(),
                 [](char c) { return std::tolower(c); });
}

void URL::setScheme(const std::string &scheme) {
  scheme_ = scheme;
  std::transform(scheme_.begin(), scheme_.end(), scheme_.begin(),
                 [](char c) { return std::tolower(c); });
}

void URL::setUserInfo(const std::string &userInfo) {
  if (userInfo.empty()) {
    user_.clear();
    password_.clear();
    return;
  }

  auto pos = userInfo.find(':');
  if (pos == userInfo.npos) {
    user_ = userInfo;
  } else {
    user_ = userInfo.substr(0, pos);
    password_ = userInfo.substr(pos + 1);
  }
}

std::string URL::userInfo() const {
  if (!isValid())
    return "";
  return user_ + (password_.empty() ? "" : ":" + password_);
}
} // namespace Http
} // namespace Darabonba
