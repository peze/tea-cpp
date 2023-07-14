#ifndef DARABONBA_ENCODE_HASH_H_
#define DARABONBA_ENCODE_HASH_H_

#include <cstdint>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <vector>

namespace Darabonba {
namespace Encode {
class Hash {
public:
  Hash(const EVP_MD *type) : ctx_(EVP_MD_CTX_new()) {
    EVP_DigestInit_ex(ctx_, type, nullptr);
  }
  Hash(const Hash &obj) : ctx_(EVP_MD_CTX_dup(obj.ctx_)) {}
  Hash(Hash &&obj) : ctx_(obj.ctx_) { obj.ctx_ = nullptr; }
  Hash &operator=(const Hash &obj) {
    if (this == &obj)
      return *this;
    ctx_ = EVP_MD_CTX_dup(obj.ctx_);
    return *this;
  }
  Hash &operator=(Hash &&obj) {
    if (this == &obj)
      return *this;
    ctx_ = obj.ctx_;
    obj.ctx_ = nullptr;
    return *this;
  }

  virtual ~Hash() { EVP_MD_CTX_free(ctx_); }

  virtual std::vector<uint8_t> final() = 0;

  virtual void update(const void *data, size_t bytes) {
    EVP_DigestUpdate(ctx_, data, bytes);
  }

  virtual Hash *clone() = 0;

protected:
  std::vector<uint8_t> final(unsigned int len) {
    std::vector<uint8_t> hash(len);
    EVP_DigestFinal_ex(
        ctx_,
        reinterpret_cast<unsigned char *>(const_cast<uint8_t *>(&hash[0])),
        &len);
    return hash;
  }

  EVP_MD_CTX *ctx_ = nullptr;
};
} // namespace Encode
} // namespace Darabonba

#endif