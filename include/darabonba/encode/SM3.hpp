#ifndef DARABONBA_ENCODE_SM3_H_
#define DARABONBA_ENCODE_SM3_H_
#include <darabonba/encode/Hash.hpp>

namespace Darabonba {
namespace Encode {
class SM3 : public Hash {
public:
  SM3() : Hash(EVP_sm3()) {}

  virtual ~SM3() {}

  virtual std::vector<uint8_t> final() override { return final(32); }

  virtual SM3 *clone() override { return new SM3(*this); }

  static std::vector<uint8_t> hash(const std::vector<uint8_t> &content) {
    SM3 hash;
    hash.update(reinterpret_cast<const void *>(&content[0]), content.size());
    return hash.final();
  }
  static std::vector<uint8_t> hash(const void *content, size_t contentSize) {
    SM3 hash;
    hash.update(content, contentSize);
    return hash.final();
  }

protected:
  using Hash::final;
};
} // namespace Encode
} // namespace Darabonba

#endif