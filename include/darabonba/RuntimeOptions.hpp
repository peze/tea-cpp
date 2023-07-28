#ifndef DARABONBA_RUNTIME_OPTIONS_H_
#define DARABONBA_RUNTIME_OPTIONS_H_

#include <darabonba/JSON.hpp>
#include <darabonba/Model.hpp>

namespace Darabonba {

class RuntimeOptions : public Model {
  friend void to_json(JSON &j, const RuntimeOptions &obj) {
    DARABONBA_PTR_TO_JSON(autoretry, autoretry_);
    DARABONBA_PTR_TO_JSON(ignoreSSL, ignoreSSL_);
    DARABONBA_PTR_TO_JSON(key, key_);
    DARABONBA_PTR_TO_JSON(cert, cert_);
    DARABONBA_PTR_TO_JSON(ca, ca_);
    DARABONBA_PTR_TO_JSON(max_attempts, maxAttempts_);
    DARABONBA_PTR_TO_JSON(backoff_policy, backoffPolicy_);
    DARABONBA_PTR_TO_JSON(backoff_period, backoffPeriod_);
    DARABONBA_PTR_TO_JSON(readTimeout, readTimeout_);
    DARABONBA_PTR_TO_JSON(connectTimeout, connectTimeout_);
    DARABONBA_PTR_TO_JSON(httpProxy, httpProxy_);
    DARABONBA_PTR_TO_JSON(httpsProxy, httpsProxy_);
    DARABONBA_PTR_TO_JSON(noProxy, noProxy_);
    DARABONBA_PTR_TO_JSON(maxIdleConns, maxIdleConns_);
    DARABONBA_PTR_TO_JSON(localAddr, localAddr_);
    DARABONBA_PTR_TO_JSON(socks5Proxy, socks5Proxy_);
    DARABONBA_PTR_TO_JSON(socks5NetWork, socks5NetWork_);
    DARABONBA_PTR_TO_JSON(keepAlive, keepAlive_);
  }
  friend void from_json(const JSON &j, RuntimeOptions &obj) {
    DARABONBA_PTR_FROM_JSON(autoretry, autoretry_);
    DARABONBA_PTR_FROM_JSON(ignoreSSL, ignoreSSL_);
    DARABONBA_PTR_FROM_JSON(key, key_);
    DARABONBA_PTR_FROM_JSON(cert, cert_);
    DARABONBA_PTR_FROM_JSON(ca, ca_);
    DARABONBA_PTR_FROM_JSON(max_attempts, maxAttempts_);
    DARABONBA_PTR_FROM_JSON(backoff_policy, backoffPolicy_);
    DARABONBA_PTR_FROM_JSON(backoff_period, backoffPeriod_);
    DARABONBA_PTR_FROM_JSON(readTimeout, readTimeout_);
    DARABONBA_PTR_FROM_JSON(connectTimeout, connectTimeout_);
    DARABONBA_PTR_FROM_JSON(httpProxy, httpProxy_);
    DARABONBA_PTR_FROM_JSON(httpsProxy, httpsProxy_);
    DARABONBA_PTR_FROM_JSON(noProxy, noProxy_);
    DARABONBA_PTR_FROM_JSON(maxIdleConns, maxIdleConns_);
    DARABONBA_PTR_FROM_JSON(localAddr, localAddr_);
    DARABONBA_PTR_FROM_JSON(socks5Proxy, socks5Proxy_);
    DARABONBA_PTR_FROM_JSON(socks5NetWork, socks5NetWork_);
    DARABONBA_PTR_FROM_JSON(keepAlive, keepAlive_);
  }

public:
  RuntimeOptions() = default;
  RuntimeOptions(const JSON &map) { from_json(map, *this); }
  ~RuntimeOptions() = default;

  virtual JSON toMap() const override {
    JSON map;
    to_json(map, *this);
    return map;
  }

  virtual void fromMap(const JSON &map) override { from_json(map, *this); }

  virtual void validate() const override {}

  bool autoretry() const { DARABONBA_PTR_GET(autoretry_, false); }
  void setAutoretry(bool autoretry) {
    DARABONBA_PTR_SET(autoretry_, autoretry);
  }

  bool ignoreSSL() const { DARABONBA_PTR_GET(ignoreSSL_, false); }
  void setIgnoreSSL(bool ignoreSSL) {
    DARABONBA_PTR_SET(ignoreSSL_, ignoreSSL);
  }

  std::string key() const { DARABONBA_PTR_GET(key_, ""); }
  void setKey(const std::string &key) { DARABONBA_PTR_SET(key_, key); }

  std::string cert() const { DARABONBA_PTR_GET(cert_, ""); }
  void setCert(const std::string &cert) { DARABONBA_PTR_SET(cert_, cert); }

  std::string ca() const { DARABONBA_PTR_GET(ca_, ""); }
  void setCa(const std::string &ca) { DARABONBA_PTR_SET(ca_, ca); }

  int64_t maxAttempts() const { DARABONBA_PTR_GET(maxAttempts_, 0); }
  void setMaxAttempts(int64_t maxAttempts) {
    DARABONBA_PTR_SET(maxAttempts_, maxAttempts);
  }

  std::string backoffPolicy() const { DARABONBA_PTR_GET(backoffPolicy_, ""); }
  void setBackoffPolicy(const std::string &backoffPolicy) {
    DARABONBA_PTR_SET(backoffPolicy_, backoffPolicy);
  }

  int64_t backoffPeriod() const { DARABONBA_PTR_GET(backoffPeriod_, 0); }
  void setBackoffPeriod(int64_t backoffPeriod) {
    DARABONBA_PTR_SET(backoffPeriod_, backoffPeriod);
  }

  int64_t readTimeout() const { DARABONBA_PTR_GET(readTimeout_, 0); }
  void setReadTimeout(int64_t readTimeout) {
    DARABONBA_PTR_SET(readTimeout_, readTimeout);
  }

  int64_t connectTimeout() const { DARABONBA_PTR_GET(connectTimeout_, 0); }
  void setConnectTimeout(int64_t connectTimeout) {
    DARABONBA_PTR_SET(connectTimeout_, connectTimeout);
  }

  std::string httpProxy() const { DARABONBA_PTR_GET(httpProxy_, ""); }
  void setHttpProxy(const std::string &httpProxy) {
    DARABONBA_PTR_SET(httpProxy_, httpProxy);
  }

  std::string httpsProxy() const { DARABONBA_PTR_GET(httpsProxy_, ""); }
  void setHttpsProxy(const std::string &httpsProxy) {
    DARABONBA_PTR_SET(httpsProxy_, httpsProxy);
  }

  std::string noProxy() const { DARABONBA_PTR_GET(noProxy_, ""); }
  void setNoProxy(const std::string &noProxy) {
    DARABONBA_PTR_SET(noProxy_, noProxy);
  }

  int64_t maxIdleConns() const { DARABONBA_PTR_GET(maxIdleConns_, 0); }
  void setMaxIdleConns(int64_t maxIdleConns) {
    DARABONBA_PTR_SET(maxIdleConns_, maxIdleConns);
  }

  std::string localAddr() const { DARABONBA_PTR_GET(localAddr_, ""); }
  void setLocalAddr(const std::string &localAddr) {
    DARABONBA_PTR_SET(localAddr_, localAddr);
  }

  std::string socks5Proxy() const { DARABONBA_PTR_GET(socks5Proxy_, ""); }
  void setSocks5Proxy(const std::string &socks5Proxy) {
    DARABONBA_PTR_SET(socks5Proxy_, socks5Proxy);
  }
  std::string socks5NetWork() const { DARABONBA_PTR_GET(socks5NetWork_, ""); }
  void setSocks5NetWork(const std::string &socks5NetWork) {
    DARABONBA_PTR_SET(socks5NetWork_, socks5NetWork);
  }

  bool keepAlive() const { DARABONBA_PTR_GET(keepAlive_, false); }
  void setKeepAlive(bool keepAlive) {
    DARABONBA_PTR_SET(keepAlive_, keepAlive);
  }

protected:
  // whether to try again
  std::shared_ptr<bool> autoretry_;

  // ignore SSL validation
  std::shared_ptr<bool> ignoreSSL_;

  // privite key for client certificate
  std::shared_ptr<std::string> key_;

  // client certificate
  std::shared_ptr<std::string> cert_;

  // server certificate
  std::shared_ptr<std::string> ca_;

  // maximum number of retries
  std::shared_ptr<int64_t> maxAttempts_;

  // backoff policy
  std::shared_ptr<std::string> backoffPolicy_;

  // backoff period
  std::shared_ptr<int64_t> backoffPeriod_;

  // read timeout
  std::shared_ptr<int64_t> readTimeout_;

  // connect timeout
  std::shared_ptr<int64_t> connectTimeout_;

  // http proxy url
  std::shared_ptr<std::string> httpProxy_;

  // https Proxy url
  std::shared_ptr<std::string> httpsProxy_;

  // agent blacklist
  std::shared_ptr<std::string> noProxy_;

  // maximum number of connections
  std::shared_ptr<int64_t> maxIdleConns_;

  // local addr
  std::shared_ptr<std::string> localAddr_;

  // SOCKS5 proxy
  std::shared_ptr<std::string> socks5Proxy_;

  // SOCKS5 netWork
  std::shared_ptr<std::string> socks5NetWork_;

  // whether to enable keep-alive
  std::shared_ptr<bool> keepAlive_;
};

} // namespace Darabonba

#endif