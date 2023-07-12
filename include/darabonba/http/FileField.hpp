#ifndef DARABONBA_HTTP_FILE_FIELD_H_
#define DARABONBA_HTTP_FILE_FIELD_H_

#include <curl/curl.h>
#include <darabonba/Core.hpp>
#include <darabonba/Exception.hpp>
#include <darabonba/Model.hpp>
#include <darabonba/Stream.hpp>
#include <memory>
#include <string>
#include <vector>

namespace Darabonba {
namespace Http {

class FileField : public Model {

  friend void to_json(JSON &j, const FileField &obj) {
    DARABONBA_PTR_TO_JSON(filename, filename_);
    DARABONBA_PTR_TO_JSON(contentType, contentType_);
    DARABONBA_PTR_TO_JSON(content, content_);
  }
  friend void from_json(const JSON &j, FileField &obj) {
    DARABONBA_PTR_FROM_JSON(filename, filename_, std::string);
    DARABONBA_PTR_FROM_JSON(contentType, contentType_, std::string);
    DARABONBA_PTR_FROM_JSON(content, content_, std::string);
  }

public:
  FileField() = default;
  ~FileField() = default;

  FileField(const JSON &map) { from_json(map, *this); }

  virtual JSON toMap() const override {
    JSON map;
    to_json(map, *this);
    return map;
  };

  virtual void fromMap(const JSON &map) override {
    validate();
    from_json(map, *this);
  }

  virtual void validate() const override {}

  std::string filename() const { return filename_ ? *filename_ : ""; }
  void setFilename(const std::string &filename) {
    if (filename_) {
      *filename_ = filename;
    } else {
      filename_ = std::make_shared<std::string>(filename);
    }
  }

  std::string contentType() const { return contentType_ ? *contentType_ : ""; }
  void setContentType(const std::string &contentType) {
    if (contentType_) {
      *contentType_ = contentType;
    } else {
      contentType_ = std::make_shared<std::string>(contentType);
    }
  }

  std::string content() const { return content_ ? *content_ : ""; }
  void setContent(const std::string &content) {
    if (content_) {
      *content_ = content;
    } else {
      content_ = std::make_shared<std::string>(content);
    }
  }

  static std::string getBoundary() { return Core::uuid(); }

  static std::shared_ptr<Stream> toFileForm(const JSON &form,
                                            const std::string &boundary);

private:
  // the name of the file
  std::shared_ptr<std::string> filename_;

  // the MIME of the file
  std::shared_ptr<std::string> contentType_;

  // the content of the file
  std::shared_ptr<std::string> content_;
};

class FileFormStream : public IStream, public std::vector<FileField> {
public:
  virtual ~FileFormStream() { curl_mime_free(mime_); }

  curl_mime *mime() const { return mime_; }
  void setMine(curl_mime *mine) { mime_ = mine; }

protected:
  curl_mime *mime_;
};
} // namespace Http
} // namespace Darabonba
#endif