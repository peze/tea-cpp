#ifndef DARABONBA_ENCODE_SHA1_H_
#define DARABONBA_ENCODE_SHA1_H_
#include <darabonba/encode/Hash.hpp>

namespace Darabonba {
namespace Encode {
class SHA1 : public Hash {
public:
  SHA1() : Hash(EVP_sha1()) {}
  virtual ~SHA1() {}

  virtual std::vector<uint8_t> final() { return final(20); }

  virtual SHA1 *clone() override { return new SHA1(*this); }

  static std::vector<uint8_t> hash(const std::vector<uint8_t> &content) {
    SHA1 hash;
    hash.update(reinterpret_cast<const void *>(&content[0]), content.size());
    return hash.final();
  }
  static std::vector<uint8_t> hash(const void *content, size_t contentSize) {
    SHA1 hash;
    hash.update(content, contentSize);
    return hash.final();
  }

protected:
  using Hash::final;
};
} // namespace Encode
} // namespace Darabonba

#endif