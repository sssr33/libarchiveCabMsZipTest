#pragma once

#include <stdexcept>
#include <libarchive/archive.h>

class LibarchiveException : public std::runtime_error {
public:
    LibarchiveException(int errorCode);
    LibarchiveException(const LibarchiveException&) = default;
    LibarchiveException(LibarchiveException&&) = default;

    LibarchiveException &operator=(const LibarchiveException&) = default;
    LibarchiveException &operator=(LibarchiveException&&) = default;

    int GetErrorCode() const;

    static void ThrowIfFailed(int errorCode);

private:
    int errorCode;
};

