#ifndef DARABONBA_HTTP_URL_H_
#define DARABONBA_HTTP_URL_H_

#include <cstdint>
#include <curl/curl.h>
#include <darabonba/http/Query.hpp>
#include <map>
#include <sstream>
#include <string>

namespace Darabonba {
namespace Http {
class URL {
public:
  URL(const std::string &);
  URL(const char *s) : URL(std::string(s)) {}
  URL() = default;

  operator std::string() const;

  URL(const URL &other) = default;
  URL(URL &&other) = default;
  ~URL() = default;

  URL &operator=(const URL &URL) = default;
  URL &operator=(URL &&other) = default;
  bool operator==(const URL &URL) const;
  bool operator!=(const URL &URL) const { return !(*this == URL); };

  const Query &query() const { return query_; }
  Query &query() { return query_; }
  std::string query(const std::string &name) const {
    auto it = query_.find(name);
    return (it != query_.end()) ? it->second : "";
  }

  const std::string &host() const { return host_; }
  void setHost(const std::string &host);

  void clear();
  bool isEmpty() const;
  bool isValid() const;

  uint16_t port() const { return port_; }
  void setPort(uint16_t port) { port_ = port; }

  const std::string &user() const { return user_; }
  void setUser(const std::string &user) { user_ = user; }
  const std::string &password() const { return password_; }
  void setPassword(const std::string &password) { password_ = password; }
  std::string userInfo() const;
  void setUserInfo(const std::string &userInfo);

  const std::string &path() const { return path_; }
  void setPath(const std::string &path) { path_ = path; }

  const std::string &scheme() const { return scheme_; }
  void setScheme(const std::string &scheme);

  std::string authority() const;
  void setAuthority(const std::string &authority);

  const std::string &fragment() const { return fragment_; }
  bool hasFragment() const { return !fragment_.empty(); }
  void setFragment(const std::string &fragment) { fragment_ = fragment; }

protected:
  enum { INVALID_PORT = 0 };

  std::string scheme_;
  std::string user_;
  std::string password_;
  std::string host_;
  std::string path_;
  uint16_t port_ = 0;
  Query query_;
  std::string fragment_;
};
} // namespace Http
} // namespace Darabonba

#endif