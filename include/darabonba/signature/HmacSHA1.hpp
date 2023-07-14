#ifndef DARABONBA_SIGNATURE_HMACSHA1_H_
#define DARABONBA_SIGNATURE_HMACSHA1_H_

#include <darabonba/signature/Hmac.hpp>

namespace Darabonba {
namespace Signature {
class HmacSHA1 : public Hmac {
public:
  HmacSHA1(const std::vector<uint8_t> &key): Hmac(EVP_sha1(), key) {}
  HmacSHA1(const void *key, size_t keyLen): Hmac(EVP_sha1(), key, keyLen) {}

  virtual std::vector<uint8_t> final() override {
    return final(20);
  }

  static std::vector<uint8_t> sign(const std::vector<uint8_t> &content, const std::vector<uint8_t> &key) {
    HmacSHA1 hmac(key);
    hmac.update(reinterpret_cast<const void *>(&content[0]), content.size());
    return hmac.final();
  }

  static std::vector<uint8_t> sign(const void *content, size_t contentSize, const void *key, size_t keyLen) {
    HmacSHA1 hmac(key, keyLen);
    hmac.update(content, contentSize);
    return hmac.final();
  }
protected:
  using Hmac::final;
};

} // namespace Crypto
} // namespace Darabonba

#endif