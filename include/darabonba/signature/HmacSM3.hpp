#ifndef DARABONBA_SIGNATURE_HMACSM3_H_
#define DARABONBA_SIGNATURE_HMACSM3_H_

#include <darabonba/signature/Hmac.hpp>

namespace Darabonba {
namespace Signature {
class HmacSM3 : public Hmac {
public:
  HmacSM3(const std::vector<uint8_t> &key) : Hmac(EVP_sm3(), key) {}
  HmacSM3(const void *key, size_t keyLen) : Hmac(EVP_sm3(), key, keyLen) {}

  virtual std::vector<uint8_t> final() override { return final(32); }

  static std::vector<uint8_t> sign(const std::vector<uint8_t> &content,
                                   const std::vector<uint8_t> &key) {
    HmacSM3 hmac(key);
    hmac.update(reinterpret_cast<const void *>(&content[0]), content.size());
    return hmac.final();
  }
  static std::vector<uint8_t> sign(const void *content, size_t contentSize,
                                   const void *key, size_t keyLen) {
    HmacSM3 hmac(key, keyLen);
    hmac.update(content, contentSize);
    return hmac.final();
  }

protected:
  using Hmac::final;
};

} // namespace Signature
} // namespace Darabonba

#endif