#ifndef DARABONBA_STRING_H_
#define DARABONBA_STRING_H_

#include <algorithm>
#include <cctype>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

namespace Darabonba {
class String {
public:
  static std::vector<std::string>
  split(const std::string &raw, const std::string &delim, int limit = -1) {
    std::vector<std::string> ret;
    std::string::size_type pos = 0;
    int i = 0;
    for (; limit < 0 || i < limit; ++i) {
      auto nextPos = raw.find(delim, pos);
      if (nextPos == std::string::npos) {
        break;
      }
      ret.emplace_back(raw.substr(pos, nextPos - pos));
      pos = nextPos + delim.size();
    }
    if (limit < 0 || i < limit) {
      ret.emplace_back(raw.substr(pos, raw.size() - pos));
    } else {
      ret.back() += raw.substr(pos, raw.size() - pos);
    }
    return ret;
  }

  static std::vector<std::string> split(std::shared_ptr<std::string> raw,
                                        std::shared_ptr<std::string> delim,
                                        std::shared_ptr<int> limit) {
    if (!raw || !delim || !limit)
      return {};
    return split(*raw, *delim, *limit);
  }

  static std::string replace(const std::string &raw, const std::string &oldStr,
                             const std::string &newStr, int count = -1) {
    if (oldStr.empty()) {
      return raw;
    }
    std::string::size_type pos = 0;
    std::string ret;
    for (int i = 0; count < 0 || i < count; ++i) {
      auto nextPos = raw.find(oldStr, pos);
      if (nextPos == std::string::npos) {
        break;
      }
      ret.append(raw, pos, nextPos - pos);
      ret.append(newStr);
      pos = nextPos + oldStr.size();
    }
    ret.append(raw, pos, raw.size() - pos);
    return ret;
  }
  static std::string replace(std::shared_ptr<std::string> raw,
                             std::shared_ptr<std::string> oldStr,
                             std::shared_ptr<std::string> newStr,
                             std::shared_ptr<int> count) {
    if (!raw || !newStr || !newStr)
      return "";
    return replace(*raw, *oldStr, *newStr, count ? *count : -1);
  }

  static bool contains(const std::string &s, const std::string &substr) {
    return s.find(substr) != std::string::npos;
  }
  static bool contains(std::shared_ptr<std::string> s,
                       std::shared_ptr<std::string> substr) {
    if (!s || !substr)
      return false;
    return s->find(*substr) != std::string::npos;
  }

  static int count(const std::string &s, const std::string &substr) {
    int count = 0;
    std::string::size_type pos = s.find(substr);
    while (pos != std::string::npos) {
      count++;
      pos = s.find(substr, pos + substr.length());
    }
    return count;
  }
  static int count(std::shared_ptr<std::string> s,
                   std::shared_ptr<std::string> substr) {
    if (!s || !substr)
      return 0;
    return count(*s, *substr);
  }

  static bool hasPrefix(const std::string &s, const std::string &prefix) {
    auto targetLength = prefix.size();
    if (s.size() < targetLength) {
      return false;
    }
    return memcmp(s.c_str(), prefix.c_str(), targetLength) == 0;
  }
  static bool hasPrefix(std::shared_ptr<std::string> s,
                        std::shared_ptr<std::string> prefix) {
    if (!s || !prefix)
      return false;
    return hasSuffix(*s, *prefix);
  }

  static bool hasSuffix(const std::string &s, const std::string &substr) {
    auto targetLength = substr.size();
    if (s.size() < targetLength) {
      return false;
    }
    return memcmp(s.c_str() + s.size() - targetLength, substr.c_str(),
                  targetLength) == 0;
  }
  static bool hasSuffix(std::shared_ptr<std::string> s,
                        std::shared_ptr<std::string> substr) {
    if (!s || !substr)
      return false;
    return hasSuffix(*s, *substr);
  }

  static int index(const std::string &s, const std::string &substr) {
    auto ret = s.find(substr);
    return ret == std::string::npos ? -1 : ret;
  }
  static int index(std::shared_ptr<std::string> s,
                   std::shared_ptr<std::string> substr) {
    if (!s || !substr)
      return -1;
    return index(*s, *substr);
  }

  static std::string toLower(const std::string &s) {
    std::string ret(s.size(), '\0');
    std::transform(s.begin(), s.end(), ret.begin(),
                   [](char c) { return std::tolower(c); });
    return ret;
  }
  static std::string toLower(std::shared_ptr<std::string> s) {
    if (!s)
      return "";
    return toLower(*s);
  }

  static std::string toUpper(const std::string &s) {
    std::string ret(s.size(), '\0');
    std::transform(s.begin(), s.end(), ret.begin(),
                   [](char c) { return std::toupper(c); });
    return ret;
  }
  static std::string toUpper(std::shared_ptr<std::string> s) {
    if (!s)
      return "";
    return toUpper(*s);
  }

  static std::string subString(const std::string &s,
                               std::string::size_type start,
                               std::string::size_type end) {
    return s.substr(start, end - start);
  }

  static std::string subString(std::shared_ptr<std::string> s,
                               std::shared_ptr<std::string::size_type> start,
                               std::shared_ptr<std::string::size_type> end) {
    if (!s)
      return "";
    else if (!start)
      return s->substr(0, *end);
    else if (!end)
      return s->substr(*start);
    else
      return s->substr(*start, *end);
  }

  static bool equals(const std::string &expect, const std::string &actual) {
    return expect == actual;
  }
  static bool equals(std::shared_ptr<std::string> expect,
                     std::shared_ptr<std::string> actual) {
    if (!expect || !actual)
      return false;
    return *expect == *actual;
  }

  static std::string trim(const std::string &s) {
    auto begin = s.begin(), end = s.end();
    for (; begin != end && std::isspace(*begin); ++begin)
      ;
    for (; begin != end && std::isspace(*std::prev(end)); --end)
      ;
    return std::string(begin, end);
  }
  static std::string trim(std::shared_ptr<std::string> s) {
    if (!s)
      return "";
    return trim(*s);
  }
};
} // namespace Darabonba

#endif
