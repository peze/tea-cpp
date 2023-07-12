#ifndef DARABONBA_STREAM_H_
#define DARABONBA_STREAM_H_

#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace Darabonba {

class IStream;

class Stream {
public:
  virtual ~Stream() {}
  static std::shared_ptr<IStream> readFromFilePath(const std::string &path);

  static std::shared_ptr<IStream> readFromBytes(std::vector<uint8_t> &raw);

  static std::shared_ptr<IStream> readFromString(const std::string &raw);

  static void reset(std::shared_ptr<Stream> raw);
};

class IStream : public Stream {
public:
  virtual ~IStream() {}
};

class ISStream : public IStream, public std::istringstream {
public:
  ISStream() = default;
  ISStream(std::istringstream &&obj) : std::istringstream(std::move(obj)) {}
  virtual ~ISStream() {}

  ISStream &operator=(std::istringstream &&obj) {
    if (this == &obj)
      return *this;
    std::istringstream::operator=(std::move(obj));
    return *this;
  }
};

class IFStream : public IStream, public std::ifstream {
public:
  IFStream() = default;
  IFStream(std::ifstream &&obj) : std::ifstream(std::move(obj)) {}
  virtual ~IFStream() {}

  IFStream &operator=(std::ifstream &&obj) {
    if (this == &obj)
      return *this;
    std::ifstream::operator=(std::move(obj));
    return *this;
  }
};

/**
 * @brief A custom IStream. You can inherit this class and implement the
 * pipeline stream.
 */
class CustomIStream : public IStream {
public:
  virtual ~CustomIStream() {}
  /**
   * @param buffer The buffer that stores data on the target end.
   * @param expectSize The data expected to be written by the target end.
   * @return The actual size of data that written to the target end. If return
   * 0, it indicates that the data that needs to written to the target end has
   * been completed.
   */
  virtual size_t write(char *buffer, size_t expectSize) = 0;
};

class OStream : public Stream {
public:
  virtual ~OStream(){};
};

class OSStream : public OStream, public std::ostringstream {
public:
  OSStream() = default;
  OSStream(std::ostringstream &&obj) : std::ostringstream(std::move(obj)) {}
  virtual ~OSStream() {}

  OSStream &operator=(std::ostringstream &&obj) {
    if (this == &obj)
      return *this;
    std::ostringstream::operator=(std::move(obj));
    return *this;
  }
};

class OFStream : public OStream, public std::ofstream {
public:
  OFStream() = default;
  OFStream(std::ofstream &&obj) : std::ofstream(std::move(obj)) {}
  virtual ~OFStream() {}

  OFStream &operator=(std::ofstream &&obj) {
    if (this == &obj)
      return *this;
    std::ofstream::operator=(std::move(obj));
    return *this;
  }
};

/**
 * @brief A custom OStream. You can inherit this class and implement the
 * pipeline stream.
 */
class CustomOStream : public OStream {
public:
  virtual ~CustomOStream() {}
  /**
   * @param buffer The buffer that stores data on the source end.
   * @param expectSize The data expected to be read from the source end.
   * @return The actual size of data that read from the source end. If return
   * 0, it indicates that the data that needs to written to the target end has
   * been completed.
   */
  virtual size_t read(char *buffer, size_t expectSize) = 0;
};

} // namespace Darabonba
#endif
