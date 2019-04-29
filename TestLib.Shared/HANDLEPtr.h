#pragma once

#include <memory>
#include <type_traits>
#include <Windows.h>

struct HANDLECloseDeleter {
    void operator()(HANDLE h);
};

typedef std::unique_ptr<std::remove_pointer<HANDLE>::type, HANDLECloseDeleter> UHANDLEClosePtr;