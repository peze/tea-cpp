#ifndef DARABONBA_TYPE_H_
#define DARABONBA_TYPE_H_
#include <cstdint>
#include <nlohmann/json.hpp>
#include <vector>
namespace Darabonba {
using Json = nlohmann::json;
// todo
using Bytes2 = nlohmann::json::binary_t;

using Bytes = std::vector<uint8_t>;
} // namespace Darabonba

#endif