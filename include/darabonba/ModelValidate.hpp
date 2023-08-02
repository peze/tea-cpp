#ifndef DARABONBA_MODEL_VALIDATE_H_
#define DARABONBA_MODEL_VALIDATE_H_

#include <darabonba/Exception.hpp>
#include <regex>

#define DARABONBA_VALIDATE_REQUIRED(attr)                                      \
  if (this->attr == nullptr) {                                                 \
    throw Darabonba::ValidateException({{"code", "RequiredValidateError"},     \
                                        {"message", #attr " is required."}});  \
  }

#define DARABONBA_VALIDATE_MAX_LENGTH(attr, length)                            \
  if (this->attr != nullptr) {                                                 \
    auto size = this->attr->size();                                            \
    if (size > (length)) {                                                     \
      throw Darabonba::ValidateException(                                      \
          {{"code", "MaxLengthValidateError"},                                 \
           {"message",                                                         \
            #attr " is exceed max-length: " + std::to_string((length))}});     \
    }                                                                          \
  }

#define DARABONBA_VALIDATE_MIN_LENGTH(attr, length)                            \
  if (this->attr != nullptr) {                                                 \
    auto size = this->attr->size();                                            \
    if (size < (length)) {                                                     \
      throw Darabonba::ValidateException(                                      \
          {{"code", "MaxLengthValidateError"},                                 \
           {"message",                                                         \
            #attr " is exceed max-length: " + std::to_string((length))}});     \
    }                                                                          \
  }

#define DARABONBA_VALIDATE_MAXIMUM(attr, val)                                  \
  if (this->attr != nullptr) {                                                 \
    const auto &value = this->attr;                                            \
    if (value > (val)) {                                                       \
      throw Darabonba::ValidateException(                                      \
          {{"code", "MaximumValidateError"},                                   \
           {"message",                                                         \
            #attr " cannot be greater than " + std::to_string((val))}});       \
    }                                                                          \
  }

#define DARABONBA_VALIDATE_MINIMUM(obj, attr, val)                             \
  if (this->attr != nullptr) {                                                 \
    const auto &value = *(this->attr);                                         \
    if (value < (val)) {                                                       \
      throw Darabonba::ValidateException(                                      \
          {{"code", "MinimumValidateError"},                                   \
           {"message",                                                         \
            #attr " cannot be less than " + std::to_string((val))}});          \
    }                                                                          \
  }

#define DARABONBA_VALIDATE_PATTERN(obj, attr, val)                             \
  if (this->attr != nullptr) {                                                 \
    const auto &value = *(this->attr);                                         \
    std::regex pattern(val, std::regex::icase);                                \
    if (!regex_search(value, pattern)) {                                       \
      throw Darabonba::ValidateException(                                      \
          {{"code", "PatternValidateError"},                                   \
           {"message", #attr " is not match " + std::to_string((val))}});      \
    }                                                                          \
  }

#endif