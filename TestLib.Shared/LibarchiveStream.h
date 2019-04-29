#pragma once
#include "HANDLEPtr.h"

#include <cstdint>
#include <vector>
#include <libarchive/archive.h>

class LibarchiveStream {
public:
    // does not take <arch> ownership
    LibarchiveStream(archive *arch, size_t bufSize, const wchar_t *filePath);
    LibarchiveStream(const LibarchiveStream&) = delete;
    LibarchiveStream(LibarchiveStream&&) = delete;
    ~LibarchiveStream();

    LibarchiveStream &operator=(const LibarchiveStream&) = delete;
    LibarchiveStream &operator=(LibarchiveStream&&) = delete;

    // can be used to handle errors
    // dtor will not throw
    void Close();

private:
    archive *arch;
    std::vector<uint8_t> buffer;
    UHANDLEClosePtr file;

    void InitFile(const wchar_t *filePath);
    void InitCallbacks();

    la_ssize_t Read(archive *arch, const void **bufferPtr);
    la_int64_t Seek(archive *arch, la_int64_t offset, int whence);
    /*int Open(archive *arch);
    int Close(archive *arch);*/

    static la_ssize_t ReadCallback(archive *arch, void *clientData, const void **buffer);
    static la_int64_t SkipCallback(archive *arch, void *clientData, la_int64_t request);
    static la_int64_t SeekCallback(archive *arch, void *clientData, la_int64_t offset, int whence);
    static int OpenCallback(archive *arch, void *clientData);
    static int CloseCallback(archive *arch, void *clientData);
};