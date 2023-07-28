#ifndef DARABONBA_MODEL_H_
#define DARABONBA_MODEL_H_
#include <darabonba/JSON.hpp>
#include <type_traits>

namespace Darabonba {

#define DARABONBA_PTR_TO_JSON(key, attr)                                       \
  if (obj.attr) {                                                              \
    j[#key] = *(obj.attr);                                                     \
  }

#define DARABONBA_TO_JSON(key, attr) j[#key] = obj.attr;

#define DARABONBA_PTR_FROM_JSON(key, attr)                                     \
  if (j.find(#key) != j.end()) {                                               \
    if (j[#key].is_null()) {                                                   \
      obj.attr = nullptr;                                                      \
    } else {                                                                   \
      obj.attr =                                                               \
          std::make_shared<std::remove_reference<decltype(*obj.attr)>::type>(  \
              j[#key]);                                                        \
    }                                                                          \
  }

#define DARABONBA_FROM_JSON(key, attr)                                         \
  if (j.find(#key) != j.end()) {                                               \
    if (!j[#key].is_null()) {                                                  \
      obj.attr = decltype(obj.attr)();                                         \
    } else {                                                                   \
      obj.attr =                                                               \
          j[#key].get<std::remove_reference<decltype(obj.attr)>::type>();      \
    }                                                                          \
  }

#define DARABONBA_PTR_SET(attr, value)                                         \
  if (this->attr) {                                                            \
    *attr = (value);                                                           \
  } else {                                                                     \
    attr =                                                                     \
        std::make_shared<std::remove_reference<decltype(*this->attr)>::type>(  \
            (value));                                                          \
  }

#define DARABONBA_PTR_GET(attr, defaultValue)                                  \
  if (this->attr) {                                                            \
    return *attr;                                                              \
  }                                                                            \
  return (defaultValue);

#define DARABONBA_GET(attr) return this->attr;

#define DARABONBA_SET(attr, value) this->attr = value;

class Model {
public:
  virtual ~Model() {}
  virtual void validate() const = 0;
  virtual JSON toMap() const = 0;
  virtual void fromMap(const JSON &) = 0;
};

} // namespace Darabonba

#endif