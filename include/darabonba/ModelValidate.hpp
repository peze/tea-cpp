#ifndef DARABONBA_MODEL_VALIDATE_H_
#define DARABONBA_MODEL_VALIDATE_H_

#include <darabonba/Exception.hpp>
#include <regex>

#define DARABONBA_VALIDATE_REQUIRED(obj, field)                                \
  if (obj.field == nullptr) {                                                  \
    throw ValidateException({{"code", "RequiredValidateError"},                \
                             {"message", #field " is required."}});            \
  }

#define DARABONBA_VALIDATE_MAX_LENGTH(obj, field, length)                      \
  if (obj.field != nullptr) {                                                  \
    auto size = obj.field.get_ref<const std::string &>().size();               \
    if (size > (length)) {                                                     \
      throw ValidateException({{"code", "MaxLengthValidateError"},             \
                               {"message", #field " is exceed max-length: " +  \
                                               std::to_string((length))}});    \
    }                                                                          \
  }

#define DARABONBA_VALIDATE_MAX_LENGTH(obj, field, length)                      \
  if (obj.field != nullptr) {                                                  \
    auto size = obj.field.get_ref<const std::string &>().size();               \
    if (size < (length)) {                                                     \
      throw ValidateException({{"code", "MaxLengthValidateError"},             \
                               {"message", #field " is exceed max-length: " +  \
                                               std::to_string((length))}});    \
    }                                                                          \
  }

#define DARABONBA_VALIDATE_MAXIMUM(obj, field, type, val)                      \
  if (obj.field != nullptr) {                                                  \
    const auto &value = obj.field.get_ref<type const &>();                     \
    if (value > (val)) {                                                       \
      throw ValidateException({{"code", "MaximumValidateError"},               \
                               {"message", #field " cannot be greater than " + \
                                               std::to_string((val))}});       \
    }                                                                          \
  }

#define DARABONBA_VALIDATE_MINIMUM(obj, field, type, val)                      \
  if (obj.field != nullptr) {                                                  \
    const auto &value = obj.field.get_ref<type const &>();                     \
    if (value < (val)) {                                                       \
      throw ValidateException({{"code", "MinimumValidateError"},               \
                               {"message", #field " cannot be less than " +    \
                                               std::to_string((val))}});       \
    }                                                                          \
  }

#define DARABONBA_VALIDATE_PATTERN(obj, field, val)                            \
  if (obj.field != nullptr) {                                                  \
    const auto &value = obj.field.get_ref<const std::string &>();              \
    std::regex pattern(val, std::regex::icase);                                \
    if (!regex_search(value, pattern)) {                                       \
      throw ValidateException(                                                 \
          {{"code", "PatternValidateError"},                                   \
           {"message", #field " is not match " + std::to_string((val))}});     \
    }                                                                          \
  }

#endif