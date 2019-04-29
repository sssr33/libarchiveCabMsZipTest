#include "LibarchiveStream.h"
#include "LibarchiveException.h"
#include "Win32Exception.h"

#include <cassert>

LibarchiveStream::LibarchiveStream(archive *arch, size_t bufSize, const wchar_t *filePath)
    : arch(arch)
{
    this->InitFile(filePath);
    this->InitCallbacks();

    this->buffer.resize(bufSize);

    int archErr = ARCHIVE_OK;

    archErr = archive_read_open1(this->arch);
    LibarchiveException::ThrowIfFailed(archErr);
}

LibarchiveStream::~LibarchiveStream() {
    if (this->arch) {
        try {
            this->Close();
        }
        catch (...) {
        }
    }
}

void LibarchiveStream::Close() {
    int archErr = ARCHIVE_OK;

    archErr = archive_read_close(this->arch);
    LibarchiveException::ThrowIfFailed(archErr);

    this->arch = nullptr;

    if (!CloseHandle(this->file.get())) {
        Win32Exception::ThrowLastError();
    }

    this->file.release(); // no more need to close it
}

void LibarchiveStream::InitFile(const wchar_t *filePath) {
    auto h = CreateFileW(filePath, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h == INVALID_HANDLE_VALUE) {
        Win32Exception::ThrowLastError();
    }

    this->file.reset(h);
}

void LibarchiveStream::InitCallbacks() {
    int archErr = ARCHIVE_OK;

    archErr = archive_read_set_read_callback(this->arch, LibarchiveStream::ReadCallback);
    LibarchiveException::ThrowIfFailed(archErr);

    archErr = archive_read_set_skip_callback(this->arch, LibarchiveStream::SkipCallback);
    LibarchiveException::ThrowIfFailed(archErr);

    archErr = archive_read_set_seek_callback(this->arch, LibarchiveStream::SeekCallback);
    LibarchiveException::ThrowIfFailed(archErr);

    /*archErr = archive_read_set_open_callback(this->arch, LibarchiveStream::OpenCallback);
    LibarchiveException::ThrowIfFailed(archErr);

    archErr = archive_read_set_close_callback(this->arch, LibarchiveStream::CloseCallback);
    LibarchiveException::ThrowIfFailed(archErr);*/

    archErr = archive_read_set_callback_data(this->arch, this);
    LibarchiveException::ThrowIfFailed(archErr);
}

la_ssize_t LibarchiveStream::Read(archive *arch, const void **bufferPtr) {
    DWORD read = 0;

    assert(bufferPtr);

    if (!ReadFile(this->file.get(), this->buffer.data(), (DWORD)this->buffer.size(), &read, nullptr)) {
        Win32Exception::ThrowLastError();
    }

    *bufferPtr = this->buffer.data();

    return (la_ssize_t)read;
}

la_int64_t LibarchiveStream::Seek(archive *arch, la_int64_t offset, int whence) {
    DWORD moveMethod;

    switch (whence) {
    case SEEK_SET:
        moveMethod = FILE_BEGIN;
        break;
    case SEEK_CUR:
        moveMethod = FILE_CURRENT;
        break;
    case SEEK_END:
        moveMethod = FILE_END;
        break;
    default:
        throw Win32Exception(ERROR_BAD_ARGUMENTS);
    }

    LARGE_INTEGER seekOffset;
    LARGE_INTEGER newPos = {};

    seekOffset.QuadPart = offset;

    if (!SetFilePointerEx(this->file.get(), seekOffset, &newPos, moveMethod)) {
        Win32Exception::ThrowLastError();
    }

    return (la_int64_t)newPos.QuadPart;
}

//int LibarchiveStream::Open(archive *arch) {
//}
//
//int LibarchiveStream::Close(archive *arch) {
//}

la_ssize_t LibarchiveStream::ReadCallback(archive *arch, void *clientData, const void **buffer) {
    auto _this = static_cast<LibarchiveStream*>(clientData);

    try {
        return _this->Read(arch, buffer);
    }
    catch (...) {
        return -1;
    }
}

la_int64_t LibarchiveStream::SkipCallback(struct archive *arch, void *clientData, la_int64_t request) {
    auto _this = static_cast<LibarchiveStream*>(clientData);

    try {
        auto pos = _this->Seek(arch, 0, SEEK_CUR);
        auto reqestedPos = pos + request;
        auto newPos = _this->Seek(arch, reqestedPos, SEEK_CUR);

        assert(newPos >= pos);

        auto skipped = newPos - pos;
        return skipped;
    }
    catch (...) {
        return -1;
    }
}

la_int64_t LibarchiveStream::SeekCallback(archive *arch, void *clientData, la_int64_t offset, int whence) {
    auto _this = static_cast<LibarchiveStream*>(clientData);

    try {
        auto newPos = _this->Seek(arch, offset, whence);
        return newPos;
    }
    catch (...) {
        return -1;
    }
}

int LibarchiveStream::OpenCallback(archive *arch, void *clientData) {
    return ARCHIVE_OK;
}

int LibarchiveStream::CloseCallback(archive *arch, void *clientData) {
    return ARCHIVE_OK;
}