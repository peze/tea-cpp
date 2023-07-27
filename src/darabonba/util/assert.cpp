#include <darabonba/Util.hpp>
#include <stdexcept>

namespace Darabonba {

bool Util::assertAsBoolean(const JSON &value) {
  if (!value.is_boolean()) {
    throw Exception("value is not a bool");
  }
  return value.get<bool>();
}

std::string Util::assertAsString(const JSON &value) {
  if (!value.is_string()) {
    throw Exception("value is not a string");
  }
  return value.get<std::string>();
}

std::vector<uint8_t> Util::assertAsBytes(const JSON &value) {
  if (!value.is_array()) {
    throw Exception("value is not a bytes");
  }
  try {
    return value.get<std::vector<uint8_t>>();
  } catch (std::exception &err) {
    throw Exception("value is not a bytes");
  }
}

int64_t Util::assertAsNumber(const JSON &value) {
  if (!value.is_number()) {
    throw Exception("value is not a bytes");
  }
  return value.get<int64_t>();
}

JSON Util::assertAsMap(const JSON &value) {
  if (!value.is_object()) {
    throw Exception("value is not a object");
  }
  return value;
}

IStream *Util::assertAsReadable(Stream *value) {

  auto ret = dynamic_cast<ISStream *>(value);
  if (ret)
    return ret;
  else
    throw Exception("value is not a object");
}

} // namespace Darabonba
