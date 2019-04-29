#include "LibarchiveException.h"

LibarchiveException::LibarchiveException(int errorCode)
    : std::runtime_error("LibarchiveException"), errorCode(errorCode)
{}

int LibarchiveException::GetErrorCode() const {
    return this->errorCode;
}

void LibarchiveException::ThrowIfFailed(int errorCode) {
    if (errorCode != ARCHIVE_OK && errorCode != ARCHIVE_EOF) {
        throw LibarchiveException(errorCode);
    }
}