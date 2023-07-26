#ifndef DARABONBA_HTTP_MCURLRESPINSEBODY_H_
#define DARABONBA_HTTP_MCURLRESPINSEBODY_H_

#include <atomic>
#include <condition_variable>
#include <darabonba/Stream.hpp>
#include <darabonba/buffer/ContiguousBuffer.hpp>
#include <darabonba/buffer/RingBuffer.hpp>
#include <darabonba/http/Response.hpp>
#include <darabonba/lock/SpinLock.hpp>
#include <numeric>

namespace Darabonba {
namespace Http {

class MCurlHttpClient;

class MCurlResponseBody : public IOStream {

  friend class MCurlHttpClient;

public:
  virtual ~MCurlResponseBody() {}

  size_t maxSize() const { return maxSize_; }
  // size_t setMaxSize(size_t maxSize) { maxSize_ = maxSize; }

  size_t readableSize() const { return readableSize_; }

  /**
   * @note This method is thread safe.
   * @note The calling thread will be blocked if there is no data in the buffer.
   */
  virtual size_t read(char *buffer, size_t expectSize) override;

  /**
   * @brief This method is thread safe.
   */
  virtual size_t write(char *buffer, size_t expectSize) override;

  void waitForDone();

  /**
   * @brief Indicate whether http responses have been accepted
   */
  bool done() const { return done_; }

  /**
   * @brief Indicate whether http response body is ready to receive.
   */
  bool ready() const { return ready_; }

  /**
   * @brief 发送一个请求，让 client 继续读取数据并放到流中。
   * 类似于 stringstream
   */
  bool fetch();

protected:
  enum { MAX_SIZE = 6000 };

  /**
   * @brief The maximum size of data stored in the body
   */
  std::atomic<size_t> maxSize_ = {MAX_SIZE};
  std::atomic<bool> done_ = {false};
  std::atomic<bool> ready_ = {false};

  // 用于等待整个数据接受完毕
  std::condition_variable doneCV_;

  std::atomic<size_t> readableSize_ = {0};

  // 用于等待流读写
  std::condition_variable streamCV_;

  // 暂时使用 ss 来表示，后面使用 ringbuffer 来降低
  Lock::SpinLock bufferlock_;
  // std::stringstream buffer_;
  Buffer::RingBuffer buffer_;

  MCurlHttpClient *client_ = nullptr;
  CURL *easyHandle_ = nullptr;
};
} // namespace Http
} // namespace Darabonba

#endif