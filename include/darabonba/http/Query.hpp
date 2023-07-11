#ifndef DARABONBA_HTTP_QUERY_H_
#define DARABONBA_HTTP_QUERY_H_
#include <initializer_list>
#include <map>
#include <string>

namespace Darabonba {
namespace Http {
class Query : public std::map<std::string, std::string> {
public:
  Query() = default;
  Query(const std::string &content);
  Query(const char *);
  Query(const std::map<std::string, std::string> &obj)
      : std::map<std::string, std::string>(obj) {}
  Query(std::map<std::string, std::string> &&obj)
      : std::map<std::string, std::string>(std::move(obj)) {}
  Query(std::initializer_list<std::pair<std::string, std::string>> list)
      : std::map<std::string, std::string>(list.begin(), list.end()) {}

  /**
   * @ref RFC 3986 section 2.3 Unreserved Characters (January 2005)
   */
  static std::string encode(const std::string &content,
                            const std::string &safe = "-_.~");
  static std::string decode(const std::string &content);
  operator std::string() const;
};

} // namespace Http
} // namespace Darabonba

#endif