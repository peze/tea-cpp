#ifndef DARABONBA_XML_H_
#define DARABONBA_XML_H_

#include <darabonba/JSON.hpp>
#include <darabonba/http/Response.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <string>

namespace Darabonba {
class XML {
public:
  static JSON parseXml(const std::string &body,
                       const Http::Response *resp = nullptr);

  static std::string toXML(const JSON &body);

protected:
};
} // namespace Darabonba

#endif
