#pragma once

#include <stdexcept>
#include <Windows.h>

class Win32Exception : public std::runtime_error {
public:
    Win32Exception(DWORD lastError);
    Win32Exception(const Win32Exception&) = default;
    Win32Exception(Win32Exception&&) = default;

    Win32Exception &operator=(const Win32Exception&) = default;
    Win32Exception &operator=(Win32Exception&&) = default;

    // https://docs.microsoft.com/en-us/windows/desktop/Debug/system-error-codes
    DWORD GetLastError() const;

    static void ThrowLastError();
    static void ThrowIfFailed();
    static void ThrowIfFailed(DWORD lastError);

private:
    DWORD lastError;
};