#include <cerrno>
#include <fstream>
#include <io.hpp>
#include <iostream>
#include <system_error>


using namespace io;


int FileBuffer::init(const char *filepath) {

    std::error_code ec;

    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {

        ec.assign(errno, std::generic_category());
        std::cerr << "Failed to open file '" << filepath << "': " << ec.message() << '\n';
        return 1;
    }

    std::streamsize filesize = file.tellg();
    file.seekg(0, std::ios::beg);

    buffer_.reset(new (std::nothrow) char[filesize + 1]);
    if (buffer_ == nullptr) {
        std::cerr << "Memory allocation for the file source buffer failed.";
        return 1;
    }

    file.read(buffer_.get(), filesize);
    if (file.fail() && !file.eof()) {
        ec.assign(errno, std::generic_category());
        std::cerr << "Error reading file '" << filepath << "': " << ec.message() << "\n";

        buffer_.reset();
        return 1;
    }

    buffer_[file.gcount()] = 0;
    return 0;
}
