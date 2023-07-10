#include <darabonba/RuntimeOptions.hpp>

using std::string;
namespace Darabonba {

bool RuntimeOptions::autoretry() const {
  if (autoretry_) {
    return *autoretry_;
  }
  return false;
}

void RuntimeOptions::setAutoretry(bool autoretry) {
  if (autoretry_) {
    *autoretry_ = autoretry;
  } else {
    autoretry_ = std::make_shared<bool>(autoretry);
  }
}

bool RuntimeOptions::ignoreSSL() const {
  if (ignoreSSL_) {
    return *ignoreSSL_;
  }
  return false;
}

void RuntimeOptions::setIgnoreSSL(bool ignoreSSL) {
  if (ignoreSSL_) {
    *ignoreSSL_ = ignoreSSL;
  } else {
    ignoreSSL_ = std::make_shared<bool>(ignoreSSL);
  }
}

string RuntimeOptions::key() const {
  if (key_) {
    return *key_;
  }
  return "";
}

void RuntimeOptions::setKey(const string &key) {
  if (key_) {
    *key_ = key;
  } else {
    key_ = std::make_shared<string>(key);
  }
}

string RuntimeOptions::cert() const {
  if (cert_) {
    return *cert_;
  }
  return "";
}

void RuntimeOptions::setCert(const string &cert) {
  if (cert_) {
    *cert_ = cert;
  } else {
    cert_ = std::make_shared<string>(cert);
  }
}

string RuntimeOptions::ca() const {
  if (ca_) {
    return *ca_;
  }
  return "";
}

void RuntimeOptions::setCa(const string &ca) {
  if (ca_) {
    *ca_ = ca;
  } else {
    ca_ = std::make_shared<string>(ca);
  }
}

int64_t RuntimeOptions::maxAttempts() const {
  if (maxAttempts_) {
    return *maxAttempts_;
  }
  return 0;
}

void RuntimeOptions::setMaxAttempts(int64_t maxAttempts) {
  if (maxAttempts_) {
    *maxAttempts_ = maxAttempts;
  } else {
    maxAttempts_ = std::make_shared<int64_t>(maxAttempts);
  }
}

string RuntimeOptions::backoffPolicy() const {
  if (backoffPolicy_) {
    return *backoffPolicy_;
  }
  return "";
}

void RuntimeOptions::setBackoffPolicy(const string &backoffPolicy) {
  if (backoffPolicy_) {
    *backoffPolicy_ = backoffPolicy;
  } else {
    backoffPolicy_ = std::make_shared<string>(backoffPolicy);
  }
}

int64_t RuntimeOptions::backoffPeriod() const {
  if (backoffPeriod_) {
    return *backoffPeriod_;
  }
  return 0;
}

void RuntimeOptions::setBackoffPeriod(int64_t backoffPeriod) {
  if (backoffPeriod_) {
    *backoffPeriod_ = backoffPeriod;
  } else {
    backoffPeriod_ = std::make_shared<int64_t>(backoffPeriod);
  }
}

int64_t RuntimeOptions::readTimeout() const {
  if (readTimeout_) {
    return *readTimeout_;
  }
  return 0;
}

void RuntimeOptions::setReadTimeout(int64_t readTimeout) {
  if (readTimeout_) {
    *readTimeout_ = readTimeout;
  } else {
    readTimeout_ = std::make_shared<int64_t>(readTimeout);
  }
}

int64_t RuntimeOptions::connectTimeout() const {
  if (connectTimeout_) {
    return *connectTimeout_;
  }
  return 0;
}

void RuntimeOptions::setConnectTimeout(int64_t connectTimeout) {
  if (connectTimeout_) {
    *connectTimeout_ = connectTimeout;
  } else {
    connectTimeout_ = std::make_shared<int64_t>(connectTimeout);
  }
}

string RuntimeOptions::httpProxy() const {
  if (httpProxy_) {
    return *httpProxy_;
  }
  return "";
}

void RuntimeOptions::setHttpProxy(const string &httpProxy) {
  if (httpProxy_) {
    *httpProxy_ = httpProxy;
  } else {
    httpProxy_ = std::make_shared<string>(httpProxy);
  }
}

string RuntimeOptions::httpsProxy() const {
  if (httpsProxy_) {
    return *httpsProxy_;
  }
  return "";
}

void RuntimeOptions::setHttpsProxy(const string &httpsProxy) {
  if (httpsProxy_) {
    *httpProxy_ = httpsProxy;
  } else {
    httpsProxy_ = std::make_shared<string>(httpsProxy);
  }
}

string RuntimeOptions::noProxy() const {
  if (noProxy_) {
    return *noProxy_;
  }
  return "";
}

void RuntimeOptions::setNoProxy(const string &noProxy) {
  if (noProxy_) {
    *noProxy_ = noProxy;
  } else {
    noProxy_ = std::make_shared<string>(noProxy);
  }
}

int64_t RuntimeOptions::maxIdleConns() const {
  if (maxIdleConns_) {
    return *maxIdleConns_;
  }
  return 0;
}

void RuntimeOptions::setMaxIdleConns(int64_t maxIdleConns) {
  if (maxIdleConns_) {
    *maxIdleConns_ = maxIdleConns;
  } else {
    maxIdleConns_ = std::make_shared<int64_t>(maxIdleConns);
  }
}

string RuntimeOptions::localAddr() const {
  if (localAddr_) {
    return *localAddr_;
  }
  return "";
}

void RuntimeOptions::setLocalAddr(const string &localAddr) {
  if (localAddr_) {
    *localAddr_ = localAddr;
  } else {
    localAddr_ = std::make_shared<string>(localAddr);
  }
}

string RuntimeOptions::socks5Proxy() const {
  if (socks5Proxy_) {
    return *socks5Proxy_;
  }
  return "";
}

void RuntimeOptions::setSocks5Proxy(const string &socks5Proxy) {
  if (socks5Proxy_) {
    *socks5Proxy_ = socks5Proxy;
  } else {
    socks5Proxy_ = std::make_shared<string>(socks5Proxy);
  }
}

string RuntimeOptions::socks5NetWork() const {
  if (socks5NetWork_) {
    return *socks5NetWork_;
  }
  return "";
}

void RuntimeOptions::setSocks5NetWork(const string &socks5NetWork) {
  if (socks5NetWork_) {
    *socks5NetWork_ = socks5NetWork;
  } else {
    socks5NetWork_ = std::make_shared<string>(socks5NetWork);
  }
}

bool RuntimeOptions::keepAlive() const {
  if (keepAlive_) {
    return *keepAlive_;
  }
  return false;
}

void RuntimeOptions::setKeepAlive(bool keepAlive) {
  if (keepAlive_) {
    *keepAlive_ = keepAlive;
  } else {
    keepAlive_ = std::make_shared<bool>(keepAlive);
  }
}

} // namespace Darabonba