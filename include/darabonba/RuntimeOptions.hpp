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
    DARABONBA_PTR_FROM_JSON(autoretry, autoretry_, bool);
    DARABONBA_PTR_FROM_JSON(ignoreSSL, ignoreSSL_, bool);
    DARABONBA_PTR_FROM_JSON(key, key_, std::string);
    DARABONBA_PTR_FROM_JSON(cert, cert_, std::string);
    DARABONBA_PTR_FROM_JSON(ca, ca_, std::string);
    DARABONBA_PTR_FROM_JSON(max_attempts, maxAttempts_, int64_t);
    DARABONBA_PTR_FROM_JSON(backoff_policy, backoffPolicy_, std::string);
    DARABONBA_PTR_FROM_JSON(backoff_period, backoffPeriod_, int64_t);
    DARABONBA_PTR_FROM_JSON(readTimeout, readTimeout_, int64_t);
    DARABONBA_PTR_FROM_JSON(connectTimeout, connectTimeout_, int64_t);
    DARABONBA_PTR_FROM_JSON(httpProxy, httpProxy_, std::string);
    DARABONBA_PTR_FROM_JSON(httpsProxy, httpsProxy_, std::string);
    DARABONBA_PTR_FROM_JSON(noProxy, noProxy_, std::string);
    DARABONBA_PTR_FROM_JSON(maxIdleConns, maxIdleConns_, int64_t);
    DARABONBA_PTR_FROM_JSON(localAddr, localAddr_, std::string);
    DARABONBA_PTR_FROM_JSON(socks5Proxy, socks5Proxy_, std::string);
    DARABONBA_PTR_FROM_JSON(socks5NetWork, socks5NetWork_, std::string);
    DARABONBA_PTR_FROM_JSON(keepAlive, keepAlive_, bool);
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

  bool autoretry() const;
  void setAutoretry(bool autoretry);
  bool ignoreSSL() const;
  void setIgnoreSSL(bool ignoreSSL);
  std::string key() const;
  void setKey(const std::string &key);
  std::string cert() const;
  void setCert(const std::string &cert);
  std::string ca() const;
  void setCa(const std::string &ca);
  int64_t maxAttempts() const;
  void setMaxAttempts(int64_t maxAttempts);
  std::string backoffPolicy() const;
  void setBackoffPolicy(const std::string &backoffPolicy);
  int64_t backoffPeriod() const;
  void setBackoffPeriod(int64_t backoffPeriod);
  int64_t readTimeout() const;
  void setReadTimeout(int64_t readTimeout);
  int64_t connectTimeout() const;
  void setConnectTimeout(int64_t connectTimeout);
  std::string httpProxy() const;
  void setHttpProxy(const std::string &httpProxy);
  std::string httpsProxy() const;
  void setHttpsProxy(const std::string &httpsProxy);
  std::string noProxy() const;
  void setNoProxy(const std::string &noProxy);
  int64_t maxIdleConns() const;
  void setMaxIdleConns(int64_t maxIdleConns);
  std::string localAddr() const;
  void setLocalAddr(const std::string &localAddr);
  std::string socks5Proxy() const;
  void setSocks5Proxy(const std::string &socks5Proxy);
  std::string socks5NetWork() const;
  void setSocks5NetWork(const std::string &socks5NetWork);
  bool keepAlive() const;
  void setKeepAlive(bool keepAlive);

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