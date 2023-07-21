#ifndef DARABONBA_MODEL_H_
#define DARABONBA_MODEL_H_
#include <darabonba/JSON.hpp>

namespace Darabonba {

#define DARABONBA_PTR_TO_JSON(key, attr)                                       \
  if (obj.attr) {                                                              \
    j[#key] = *(obj.attr);                                                     \
  }

#define DARABONBA_TO_JSON(key, attr) j[#key] = obj.attr;

#define DARABONBA_PTR_FROM_JSON(key, attr, type)                               \
  if (j.find(#key) != j.end()) {                                               \
    if (j[#key].is_null()) {                                                   \
      obj.attr = nullptr;                                                      \
    } else {                                                                   \
      obj.attr = std::make_shared<type>(j[#key]);                              \
    }                                                                          \
  }

#define DARABONBA_FROM_JSON(key, attr, type)                                   \
  if (j.find(#key) != j.end()) {                                               \
    if (!j[#key].is_null()) {                                                  \
      obj.attr = decltype(obj.attr)();                                         \
    } else {                                                                   \
      obj.attr = j[#key].get<type>();                                          \
    }                                                                          \
  }

#define DARABONBA_PTR_SET(attr, type, value)                                   \
  if (this->attr) {                                                            \
    *attr = (value);                                                           \
  } else {                                                                     \
    attr = std::make_shared<type>((value));                                    \
  }

#define DARABONBA_PTR_GET(attr, defaultValue)                                  \
  if (this->attr) {                                                            \
    return *attr;                                                              \
  }                                                                            \
  return (defaultValue);

class Model {
public:
  virtual ~Model() {}
  virtual void validate() const = 0;
  virtual JSON toMap() const = 0;
  virtual void fromMap(const JSON &) = 0;
};

} // namespace Darabonba

#endif