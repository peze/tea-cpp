#include <darabonba/Stream.hpp>

namespace Darabonba {
std::shared_ptr<IStream> Stream::readFromFilePath(const std::string &path) {
  return std::shared_ptr<IStream>(
      new IFStream(std::ifstream(path, std::ios::binary)));
}

std::shared_ptr<IStream> Stream::readFromBytes(std::vector<uint8_t> &raw) {
  std::string s(raw.begin(), raw.end());
  auto p = new ISStream(std::istringstream(std::move(s)));
  return std::shared_ptr<IStream>(p);
}

std::shared_ptr<IStream> Stream::readFromString(const std::string &raw) {
  auto p = new ISStream(std::istringstream(raw));
  return std::shared_ptr<IStream>(p);
}

void Stream::reset(std::shared_ptr<Stream> raw) {
  if (raw == nullptr)
    return;
  auto fs = dynamic_cast<IFStream *>(raw.get());
  if (fs) {
    fs->seekg(0);
    return;
  }
  auto ss = dynamic_cast<ISStream *>(raw.get());
  if (ss) {
    ss->seekg(0);
    return;
  }
}
} // namespace Darabonba
