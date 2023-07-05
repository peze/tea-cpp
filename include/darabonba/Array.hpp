#ifndef DARABONBA_ARRAY_H_
#define DARABONBA_ARRAY_H_

#include <algorithm>
#include <cmath>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace Darabonba {
class Array {
public:
  static std::vector<std::string> split(const std::vector<std::string> &raw,
                                        int index, int limit) {
    index = std::max(index, 0);
    if (static_cast<size_t>(index) >= raw.size() || limit <= index)
      return {};
    limit = std::min(std::max(limit, 0), static_cast<int>(raw.size()));
    return std::vector<std::string>(raw.begin() + index, raw.begin() + limit);
  }
  static std::vector<std::string>
  split(std::shared_ptr<std::vector<std::string>> raw,
        std::shared_ptr<int> index, std::shared_ptr<int> limit) {
    return raw && limit ? split(*raw, *index, *limit)
                        : std::vector<std::string>();
  }

  static bool contains(const std::vector<std::string> &raw,
                       const std::string &str) {
    return std::find(raw.begin(), raw.end(), str) != raw.end();
  }
  static bool contains(std::shared_ptr<std::vector<std::string>> raw,
                       std::shared_ptr<std::string> str) {
    return raw && str ? contains(raw, str) : false;
  }

  static int index(const std::vector<std::string> &raw,
                   const std::string &str) {
    auto pos = std::find(raw.begin(), raw.end(), str);
    return pos == raw.end() ? -1 : pos - raw.begin();
  }
  static int index(std::shared_ptr<std::vector<std::string>> raw,
                   std::shared_ptr<std::string> str) {
    return raw && str ? index(*raw, *str) : -1;
  }

  static int size(const std::vector<std::string> &raw) { return raw.size(); }
  static int size(std::shared_ptr<std::vector<std::string>> raw) {
    return raw ? raw->size() : 0;
  }

  static const std::string &get(const std::vector<std::string> &raw,
                                int index) {
    if (index < 0 || static_cast<size_t>(index) >= raw.size())
      throw std::out_of_range("Invalid index.");
    return raw[index];
  }
  static const std::string &get(std::shared_ptr<std::vector<std::string>> raw,
                                std::shared_ptr<int> index) {
    if (raw && index)
      return get(*raw, *index);
    throw std::invalid_argument("Null pointer.");
  }

  template <typename ForwardIter>
  static std::string join(ForwardIter first, ForwardIter last,
                          const std::string &delim) {
    std::string ret;
    while (first != last) {
      ret += (*first++) + delim;
    }
    ret.resize(ret.size() - delim.size());
    return ret;
  }

  static std::string join(const std::vector<std::string> &array,
                          const std::string &delim) {
    return join(array.begin(), array.end(), delim);
  }

  static std::string join(std::shared_ptr<std::vector<std::string>> raw,
                          std::shared_ptr<std::string> sep) {
    if (!raw)
      return "";
    else if (!sep)
      return join(raw->begin(), raw->end(), "");
    return join(raw->begin(), raw->end(), *sep);
  }

  static std::vector<std::string> concat(const std::vector<std::string> &raw,
                                         const std::vector<std::string> &sep) {
    std::vector<std::string> ret = raw;
    std::copy(sep.begin(), sep.end(), std::back_inserter(ret));
    return ret;
  }
  static std::vector<std::string>
  concat(std::shared_ptr<std::vector<std::string>> raw,
         std::shared_ptr<std::vector<std::string>> sep) {
    return concat(raw ? *raw : std::vector<std::string>(),
                  sep ? *sep : std::vector<std::string>());
  }

  static std::vector<std::string> &ascSort(std::vector<std::string> &raw) {
    std::sort(raw.begin(), raw.end());
    return raw;
  }
  static std::vector<std::string> &
  ascSort(std::shared_ptr<std::vector<std::string>> raw) {
    if (!raw)
      throw std::invalid_argument("Null pointer.");
    return ascSort(*raw);
  }

  static std::vector<std::string> &descSort(std::vector<std::string> &raw) {
    std::sort(raw.begin(), raw.end(), std::greater<std::string>());
    return raw;
  }
  static std::vector<std::string> &
  descSort(std::shared_ptr<std::vector<std::string>> raw) {
    if (!raw)
      throw std::invalid_argument("Null pointer.");
    return descSort(*raw);
  }

  template <typename T>
  static void append(std::vector<T> &array, const T &item) {
    array.emplace_back(item);
  }
};

} // namespace Darabonba

#endif
