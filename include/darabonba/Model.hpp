#ifndef DARABONBA_MODEL_H_
#define DARABONBA_MODEL_H_
#include <darabonba/Type.hpp>
#include <type_traits>

namespace Darabonba {

#define DARABONBA_PTR_TO_JSON(key, attr)                                       \
  if (obj.attr) {                                                              \
    j[#key] = *(obj.attr);                                                     \
  }

#define DARABONBA_TO_JSON(key, attr) j[#key] = obj.attr;

#define DARABONBA_PTR_FROM_JSON(key, attr)                                     \
  if (j.count(#key)) {                                                         \
    if (j[#key].is_null()) {                                                   \
      obj.attr = nullptr;                                                      \
    } else {                                                                   \
      using Type = std::remove_reference<decltype(*obj.attr)>::type;           \
      obj.attr = std::make_shared<Type>(j[#key].get<Type>());                  \
    }                                                                          \
  }

#define DARABONBA_FROM_JSON(key, attr)                                         \
  if (j.count(#key)) {                                                         \
    if (!j[#key].is_null()) {                                                  \
      obj.attr = decltype(obj.attr)();                                         \
    } else {                                                                   \
      obj.attr =                                                               \
          j[#key].get<std::remove_reference<decltype(obj.attr)>::type>();      \
    }                                                                          \
  }

#define DARABONBA_PTR_SET_VALUE(attr, value)                                   \
  if (this->attr) {                                                            \
    *attr = (value);                                                           \
  } else {                                                                     \
    attr =                                                                     \
        std::make_shared<std::remove_reference<decltype(*this->attr)>::type>(  \
            (value));                                                          \
  }                                                                            \
  return *this;

#define DARABONBA_PTR_SET_RVALUE(attr, value)                                  \
  if (this->attr) {                                                            \
    *attr = std::move(value);                                                  \
  } else {                                                                     \
    attr =                                                                     \
        std::make_shared<std::remove_reference<decltype(*this->attr)>::type>(  \
            std::move(value));                                                 \
  }                                                                            \
  return *this;

#define DARABONBA_PTR_GET_DEFAULT(attr, ...)                                   \
  if (this->attr) {                                                            \
    return *attr;                                                              \
  }                                                                            \
  return __VA_ARGS__;

#define DARABONBA_PTR_GET(attr) return *this->attr;

#define DARABONBA_GET(attr) return this->attr;

#define DARABONBA_SET_VALUE(attr, value)                                       \
  this->attr = value;                                                          \
  return *this;

#define DARABONBA_SET_RVALUE(attr, value)                                      \
  this->attr = std::move(value);                                               \
  return *this;

class Model {
public:
  virtual ~Model() {}
  virtual void validate() const = 0;
  virtual Json toMap() const = 0;
  virtual void fromMap(const Json &) = 0;
  virtual bool empty() const = 0;
};

} // namespace Darabonba

#endif