#include <darabonba/core.hpp>

#include <boost/algorithm/string/join.hpp>
#include <boost/chrono.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread/thread.hpp>

#include <cpprest/filestream.h>
#include <cpprest/http_client.h>
#include <fstream>

#include <cpprest/containerstream.h> // Async streams backed by STL containers
#include <cpprest/interopstream.h> // Bridges for integrating Async streams with STL and WinRT streams
#include <cpprest/producerconsumerstream.h> // Async streams for producer consumer scenarios
#include <cpprest/rawptrstream.h> // Async streams backed by raw pointer to memory
#include <cpprest/uri.h>          // URI library
#include <cpprest/ws_client.h>    // WebSocket client

using namespace std;

void Darabonba::Core::sleep(int *sleep_time) {
  int s = nullptr == sleep_time ? 0 : *sleep_time;
  boost::this_thread::sleep_for(boost::chrono::seconds(s));
}

int Darabonba::Core::getBackoffTime(boost::any *backoff, int *retry_times) {
  if (nullptr == backoff) {
    return 0;
  }
  int rt = nullptr == retry_times ? 0 : *retry_times;
  int back_off_time = 0;
  map<string, boost::any> backoff_val =
      boost::any_cast<map<string, boost::any>>(*backoff);
  string policy = "no";
  if (backoff_val.find("policy") != backoff_val.end()) {
    policy = boost::any_cast<string>(backoff_val["policy"]);
  }
  if (policy == "no") {
    return back_off_time;
  }

  if (backoff_val.find("period") != backoff_val.end()) {
    int period = boost::any_cast<int>(backoff_val.at("period"));
    back_off_time = period;
    if (back_off_time <= 0) {
      return rt;
    }
  }
  return back_off_time;
}

bool Darabonba::Core::isRetryable(exception *ex) {
  try {
    auto *e = boost::current_exception_cast<Darabonba::Error>();
    return e != nullptr;
  } catch (exception &e) {
    return false;
  }
}

bool Darabonba::Core::isRetryable(boost::exception *ex) {
  try {
    auto *e = boost::current_exception_cast<Darabonba::Error>();
    return e != nullptr;
  } catch (exception &e) {
    return false;
  }
}

bool Darabonba::Core::allowRetry(boost::any *retry, int *retry_times,
                                 int *now) {
  if (retry == nullptr) {
    return false;
  }
  int r = nullptr == retry_times ? 0 : *retry_times;
  map<string, boost::any> retry_val =
      boost::any_cast<map<string, boost::any>>(*retry);
  if (retry_val.empty()) {
    return false;
  }
  if (retry_val.find("maxAttempts") != retry_val.end()) {
    boost::any max_attempts_any = retry_val.at("maxAttempts");
    int maxAttempts = 0;
    if (typeid(int) == max_attempts_any.type()) {
      maxAttempts = boost::any_cast<int>(retry_val.at("maxAttempts"));
    }
    return maxAttempts >= r;
  }
  delete now;

  return false;
}

Darabonba::Response Darabonba::Core::doAction(const Darabonba::Request &req) {
  map<string, boost::any> runtime;
  return Darabonba::Core::doAction(req, runtime);
}

string lowercase(string str) {
  transform(str.begin(), str.end(), str.begin(),
            [](unsigned char c) { return tolower(c); });
  return str;
}

string uppercase(string str) {
  transform(str.begin(), str.end(), str.begin(),
            [](unsigned char c) { return toupper(c); });
  return str;
}

string url_encode(const string &str) {
  ostringstream escaped;
  escaped.fill('0');
  escaped << hex;

  for (char c : str) {
    if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
      escaped << c;
      continue;
    }
    escaped << uppercase;
    escaped << '%' << setw(2) << int((unsigned char)c);
    escaped << nouppercase;
  }

  return escaped.str();
}

string http_query_str(map<string, string> query) {
  string str;
  typedef map<string, string>::iterator MapIterator;
  if (!query.empty()) {
    vector<string> keys;
    keys.reserve(query.size());
    for (auto &it : query) {
      keys.push_back(it.first);
    }
    sort(keys.begin(), keys.end());
    vector<string> arr;
    for (const auto &k : keys) {
      if (query[k].empty()) {
        continue;
      }
      string tmp = k + "=" + url_encode(query[k]);
      arr.push_back(tmp);
    }
    str = boost::join(arr, "&");
  }
  return str;
}

string compose_url(const string &host, const map<string, string> &params,
                   const string &pathname, const string &schema,
                   const string &port) {
  string url = schema + "://" + host;
  url = lowercase(url);
  if (port != "80" && port != "443") {
    url = url + ":" + port;
  }
  url = url + pathname;
  if (!params.empty()) {
    string queryString = http_query_str(params);
    if (url.find('?') != string::npos) {
      if (url.back() != '&') {
        url = url + "&" + queryString;
      } else {
        url = url + queryString;
      }
    } else {
      url = url + "?" + queryString;
    }
  }
  return url;
}

Darabonba::Response
Darabonba::Core::doAction(const Darabonba::Request &req,
                           map<string, boost::any> runtime) {
  string protocol = req.protocol;
  string port = to_string(req.port);
  string method = uppercase(req.method);
  string pathname = req.pathname;
  map<string, string> query;
  for (const auto &param : req.query) {
    string key = param.first;
    string val = param.second;
    query.insert(pair<string, string>(key, val));
  }
  string body = req.body;
  map<string, string> headers = req.headers;
  string host = req.host;
  if (headers.find("host") != headers.end()) {
    host = headers["host"];
  }
  string url = compose_url(host, query, pathname, protocol, port);
  web::http::client::http_client_config cfg;
  int timeout = 0;
  if (runtime.find("readTimeout") != runtime.end()) {
    timeout = boost::any_cast<int>(runtime.at("readTimeout"));
  }
  if (runtime.find("connectTimeout") != runtime.end()) {
    timeout += boost::any_cast<int>(runtime.at("connectTimeout"));
  }
  if (timeout == 0) {
    timeout = 30;
  }
  std::chrono::microseconds t(timeout);
  cfg.set_timeout(utility::seconds(timeout));

  web::http::client::http_client client(
      web::uri(utility::conversions::to_string_t(url)), cfg);
  web::http::http_response response;
  response = client.request(method).get();
  if (nullptr != getenv("DEBUG")) {
    printf("status_code : %hu\n", response.status_code());
    printf("response : %s\n", response.to_string().c_str());
  }
  Darabonba::Response dara_response;
  concurrency::streams::stringstreambuf buffer;
  response.body().read_to_end(buffer).get();
  Concurrency::streams::basic_istream<unsigned char> outStringBuffer(buffer);
  dara_response.body = outStringBuffer;
  dara_response.statusCode = response.status_code();
  return dara_response;
}
Darabonba::Error::Error() = default;
Darabonba::Error::Error(map<string, string> error_info) {
  if (error_info.find("message") != error_info.end()) {
    message = error_info.at("message");
  }
  if (error_info.find("code") != error_info.end()) {
    code = error_info.at("code");
  }
  if (error_info.find("data") != error_info.end()) {
    data = error_info.at("data");
  }
  if (error_info.find("name") != error_info.end()) {
    name = error_info.at("name");
  }
}
