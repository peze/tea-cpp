#ifndef DARABONBA_SIGNATURE_SIGNER_H_
#define DARABONBA_SIGNATURE_SIGNER_H_

#include <darabonba/encode/MD5.hpp>
#include <darabonba/signature/HmacMD5.hpp>
#include <darabonba/signature/HmacSHA1.hpp>
#include <darabonba/signature/HmacSHA256.hpp>
#include <darabonba/signature/HmacSM3.hpp>

namespace Darabonba {

namespace Signature {
class Signer {
public:
  static Bytes HmacSHA1Sign(const std::string &stringToSign,
                            const std::string &secret) {
    return HmacSHA1::sign(reinterpret_cast<const void *>(stringToSign.c_str()),
                          stringToSign.size(),
                          reinterpret_cast<const void *>(secret.c_str()),
                          secret.size());
  }

  static Bytes HmacSHA1SignByBytes(const std::string &stringToSign,
                                   const Bytes &secret) {
    return HmacSHA1::sign(reinterpret_cast<const void *>(stringToSign.c_str()),
                          stringToSign.size(),
                          reinterpret_cast<const void *>(&secret[0]),
                          secret.size());
  }

  static Bytes HmacSHA256Sign(const std::string &stringToSign,
                              const std::string &secret) {
    return HmacSHA256::sign(
        reinterpret_cast<const void *>(stringToSign.c_str()),
        stringToSign.size(), reinterpret_cast<const void *>(secret.c_str()),
        secret.size());
  }

  static Bytes HmacSHA256SignByBytes(const std::string &stringToSign,
                                     const Bytes &secret) {
    return HmacSHA256::sign(
        reinterpret_cast<const void *>(stringToSign.c_str()),
        stringToSign.size(), reinterpret_cast<const void *>(&secret[0]),
        secret.size());
  }

  static Bytes HmacSM3Sign(const std::string &stringToSign,
                           const std::string &secret) {
    return HmacSM3::sign(reinterpret_cast<const void *>(stringToSign.c_str()),
                         stringToSign.size(),
                         reinterpret_cast<const void *>(secret.c_str()),
                         secret.size());
  }

  static Bytes HmacSM3SignByBytes(const std::string &stringToSign,
                                  const Bytes &secret) {
    return HmacSM3::sign(reinterpret_cast<const void *>(stringToSign.c_str()),
                         stringToSign.size(),
                         reinterpret_cast<const void *>(&secret[0]),
                         secret.size());
  }

  static Bytes SHA256withRSASign(const std::string &stringToSign,
                                 const std::string &secret) {
    return HmacSM3::sign(reinterpret_cast<const void *>(stringToSign.c_str()),
                         stringToSign.size(),
                         reinterpret_cast<const void *>(&secret[0]),
                         secret.size());
  }

  static Bytes MD5Sign(const std::string &stringToSign) {
    return Encode::MD5::hash(
        reinterpret_cast<const void *>(stringToSign.c_str()),
        stringToSign.size());
  }

  static Bytes MD5SignForBytes(const Bytes &bytesToSign) {
    return Encode::MD5::hash(reinterpret_cast<const void *>(&bytesToSign[0]),
                             bytesToSign.size());
  }
};
} // namespace Signature
} // namespace Darabonba
#endif