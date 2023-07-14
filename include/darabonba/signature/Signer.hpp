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
  static std::vector<uint8_t> HmacSHA1Sign(const std::string &stringToSign,
                                           const std::string &secret) {
    return HmacSHA1::sign(reinterpret_cast<const void *>(stringToSign.c_str()),
                          stringToSign.size(),
                          reinterpret_cast<const void *>(secret.c_str()),
                          secret.size());
  }

  static std::vector<uint8_t>
  HmacSHA1SignByBytes(const std::string &stringToSign,
                      const std::vector<uint8_t> &secret) {
    return HmacSHA1::sign(reinterpret_cast<const void *>(stringToSign.c_str()),
                          stringToSign.size(),
                          reinterpret_cast<const void *>(&secret[0]),
                          secret.size());
  }

  static std::vector<uint8_t> HmacSHA256Sign(const std::string &stringToSign,
                                             const std::string &secret) {
    return HmacSHA256::sign(
        reinterpret_cast<const void *>(stringToSign.c_str()),
        stringToSign.size(), reinterpret_cast<const void *>(secret.c_str()),
        secret.size());
  }

  static std::vector<uint8_t>
  HmacSHA256SignByBytes(const std::string &stringToSign,
                        const std::vector<uint8_t> &secret) {
    return HmacSHA256::sign(
        reinterpret_cast<const void *>(stringToSign.c_str()),
        stringToSign.size(), reinterpret_cast<const void *>(&secret[0]),
        secret.size());
  }

  static std::vector<uint8_t> HmacSM3Sign(const std::string &stringToSign,
                                          const std::string &secret) {
    return HmacSM3::sign(reinterpret_cast<const void *>(stringToSign.c_str()),
                         stringToSign.size(),
                         reinterpret_cast<const void *>(secret.c_str()),
                         secret.size());
  }

  static std::vector<uint8_t>
  HmacSM3SignByBytes(const std::string &stringToSign,
                     const std::vector<uint8_t> &secret) {
    return HmacSM3::sign(reinterpret_cast<const void *>(stringToSign.c_str()),
                         stringToSign.size(),
                         reinterpret_cast<const void *>(&secret[0]),
                         secret.size());
  }

  static std::vector<uint8_t> SHA256withRSASign(const std::string &stringToSign,
                                                const std::string &secret) {
    return HmacSM3::sign(reinterpret_cast<const void *>(stringToSign.c_str()),
                         stringToSign.size(),
                         reinterpret_cast<const void *>(&secret[0]),
                         secret.size());
  }

  static std::vector<uint8_t> MD5Sign(const std::string &stringToSign) {
    return Encode::MD5::hash(
        reinterpret_cast<const void *>(stringToSign.c_str()),
        stringToSign.size());
  }

  static std::vector<uint8_t>
  MD5SignForBytes(const std::vector<uint8_t> &bytesToSign) {
    return Encode::MD5::hash(reinterpret_cast<const void *>(&bytesToSign[0]),
                             bytesToSign.size());
  }
};
} // namespace Signature
} // namespace Darabonba
#endif