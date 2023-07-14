#ifndef DARABONBA_ENCODE_SHA256_H_
#define DARABONBA_ENCODE_SHA256_H_

#include <darabonba/encode/Hash.hpp>

namespace Darabonba {
namespace Encode {
class SHA256 : public Hash {
public:
  SHA256() : Hash(EVP_sha256()) {}

  virtual ~SHA256() {}

  virtual std::vector<uint8_t> final() override { return final(32); }

  virtual SHA256 *clone() override { return new SHA256(*this); }

  static std::vector<uint8_t> hash(const std::vector<uint8_t> &content) {
    SHA256 hash;
    hash.update(reinterpret_cast<const void *>(&content[0]), content.size());
    return hash.final();
  }
  static std::vector<uint8_t> hash(const void *content, size_t contentSize) {
    SHA256 hash;
    hash.update(content, contentSize);
    return hash.final();
  }

protected:
  using Hash::final;
};
} // namespace Encode
} // namespace Darabonba

#endif