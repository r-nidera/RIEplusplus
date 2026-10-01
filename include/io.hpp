#pragma once

#include <memory>

namespace io {

class FileBuffer {
    std::unique_ptr<char[]> buffer_;

  public:
    int init(const char *filepath);
    const char *contents() const { return buffer_.get(); };
};


} // namespace io
