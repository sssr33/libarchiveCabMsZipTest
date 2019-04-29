#include "Win32Exception.h"

#include <cassert>

Win32Exception::Win32Exception(DWORD lastError)
    : std::runtime_error("Win32Exception"), lastError(lastError)
{
    assert(this->lastError != ERROR_SUCCESS);
}

DWORD Win32Exception::GetLastError() const {
    return this->lastError;
}

void Win32Exception::ThrowLastError() {
    DWORD lastErr = ::GetLastError();
    assert(lastErr != ERROR_SUCCESS);
    throw Win32Exception(lastErr);
}

void Win32Exception::ThrowIfFailed() {
    DWORD lastErr = ::GetLastError();
    Win32Exception::ThrowIfFailed(lastErr);
}

void Win32Exception::ThrowIfFailed(DWORD lastError) {
    if (lastError != ERROR_SUCCESS) {
        throw Win32Exception(lastError);
    }
}