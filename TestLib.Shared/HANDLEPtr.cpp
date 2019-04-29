#include "HANDLEPtr.h"

void HANDLECloseDeleter::operator()(HANDLE h) {
    // do not throw because we're in a dtor
    CloseHandle(h);
}