#ifndef DARABONBA_SIGNATURE_HMACMD5_H_
#define DARABONBA_SIGNATURE_HMACMD5_H_

#include <darabonba/signature/Hmac.hpp>

namespace Darabonba {
namespace Signature {
class HmacMD5 : public Hmac {
public:
  HmacMD5(const std::vector<uint8_t> &key) : Hmac(EVP_md5(), key) {}
  HmacMD5(const void *key, size_t keyLen) : Hmac(EVP_md5(), key, keyLen) {}

  virtual std::vector<uint8_t> final() override { return final(16); }

  static std::vector<uint8_t> sign(const std::vector<uint8_t> &content,
                                   const std::vector<uint8_t> &key) {
    HmacMD5 hmac(key);
    hmac.update(reinterpret_cast<const void *>(&content[0]), content.size());
    return hmac.final();
  }

  static std::vector<uint8_t> sign(const void *content, size_t contentSize,
                                   const void *key, size_t keyLen) {
    HmacMD5 hmac(key, keyLen);
    hmac.update(content, contentSize);
    return hmac.final();
  }

protected:
  using Hmac::final;
};

} // namespace Signature
} // namespace Darabonba

#endif