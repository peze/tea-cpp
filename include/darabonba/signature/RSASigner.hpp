#ifndef darabonba_signature_rsa_h_
#define darabonba_signature_rsa_h_

#include <cstdint>
#include <darabonba/encode/Hash.hpp>
#include <memory>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <string>
#include <vector>

namespace Darabonba {
namespace Signature {
class RSASigner {
public:
  RSASigner(const void *privatePemKey, size_t keyLen,
            std::unique_ptr<Encode::Hash> hash)
      : hash_(std::move(hash)) {
    BIO *bioKey = BIO_new_mem_buf(privatePemKey, keyLen);
    if (!bioKey) {
      return;
    }
    PEM_read_bio_PrivateKey(bioKey, &pkey_, nullptr, nullptr);
    if (!pkey_) {
      return;
    }
    ctx_ = EVP_PKEY_CTX_new(pkey_, nullptr);
    if (!ctx_) {
      return;
    }
  }
  RSASigner(const std::string &privatePemKey,
            std::unique_ptr<Encode::Hash> hash)
      : RSASigner(reinterpret_cast<const void *>(privatePemKey.c_str()),
                  privatePemKey.size(), std::move(hash)) {}

  RSASigner(const RSASigner &) = delete;
  RSASigner(RSASigner &&) = delete;
  RSASigner &operator=(const RSASigner &) = delete;
  RSASigner &operator=(RSASigner &&) = delete;
  //   RSASigner(const RSASigner &obj)
  //       : pkey_(EVP_PKEY_dup(obj.pkey_)), ctx_(EVP_PKEY_CTX_dup(obj.ctx_)),
  //         hash_(obj.hash_->clone()) {}

  //   RSASigner(RSASigner &&obj)
  //       : pkey_(obj.pkey_), ctx_(obj.ctx_), hash_(std::move(obj.hash_)) {
  //     obj.pkey_ = nullptr;
  //     obj.ctx_ = nullptr;
  //     obj.hash_ = nullptr;
  //   }

  //   RSASigner &operator=(const RSASigner &obj) {
  //     if (this == &obj)
  //       return *this;
  //     pkey_ = EVP_PKEY_dup(obj.pkey_);
  //     ctx_ = EVP_PKEY_CTX_dup(obj.ctx_);
  //     hash_ = std::unique_ptr<Encode::Hash>(obj.hash_->clone());
  //   }

  //   RSASigner &operator=(RSASigner &&obj) {
  //     if (this == &obj)
  //       return *this;
  //     pkey_ = obj.pkey_;
  //     ctx_ = obj.ctx_;
  //     hash_ = std::move(obj.hash_);
  //     obj.pkey_ = nullptr;
  //     obj.ctx_ = nullptr;
  //     obj.hash_ = nullptr;
  //   }

  virtual ~RSASigner() {
    EVP_PKEY_CTX_free(ctx_);
    EVP_PKEY_free(pkey_);
  }

  virtual Bytes final() {
    auto md = hash_->final();
    size_t resLen;
    if (EVP_PKEY_sign(ctx_, nullptr, &resLen,
                      reinterpret_cast<const unsigned char *>(&md[0]),
                      md.size()) <= 0) {
      return {};
    }
    Bytes ret;
    ret.resize(resLen);
    if (EVP_PKEY_sign(ctx_, reinterpret_cast<unsigned char *>(&ret[0]), &resLen,
                      reinterpret_cast<const unsigned char *>(&md[0]),
                      md.size()) <= 0) {
      return {};
    }
    return ret;
  }

  virtual void update(const void *data, size_t bytes) {
    hash_->update(data, bytes);
  }

  static Bytes sign(const void *content, size_t contentSize, const void *key,
                    size_t keyLen, std::unique_ptr<Encode::Hash> hash) {
    RSASigner signer(key, keyLen, std::move(hash));
    signer.update(content, contentSize);
    return signer.final();
  }

protected:
  EVP_PKEY *pkey_;
  EVP_PKEY_CTX *ctx_;
  std::unique_ptr<Encode::Hash> hash_;
};
} // namespace Signature
} // namespace Darabonba

#endif