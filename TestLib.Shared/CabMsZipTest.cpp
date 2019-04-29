#include "CabMsZipTest.h"
#include "LibarchivePtr.h"
#include "LibarchiveStream.h"
#include "LibarchiveException.h"
#include "Win32Exception.h"

#include <cstdint>
#include <inttypes.h>

void CabMsZipTest::Run() {
    int r;

    auto a = ULibarchiveReadPtr(archive_read_new());

    r = archive_read_support_compression_all(a.get());
    LibarchiveException::ThrowIfFailed(r);

    r = archive_read_support_format_all(a.get());
    LibarchiveException::ThrowIfFailed(r);

    r = archive_read_support_filter_all(a.get());
    LibarchiveException::ThrowIfFailed(r);

    auto cabFilePath = CabMsZipTest::CreateCABFile();

    // Wiht 4kb error is likely to happen.
    // Default libarchive file stream implementation internally creates buffer of 64kb if requested size is less.
    LibarchiveStream archStream(a.get(), 1024 * 4, cabFilePath.c_str());

    int64_t totalReadSize = 0;
    int64_t totalDiscoveredSize = 0;

    try {
        archive_entry *ae = nullptr;

        while (true) {
            r = archive_read_next_header(a.get(), &ae);

            if (r == ARCHIVE_EOF) {
                break;
            }
            if (r == ARCHIVE_RETRY) {
                continue;
            }
            LibarchiveException::ThrowIfFailed(r);

            totalDiscoveredSize += archive_entry_size(ae);

            const void *buf;
            size_t size = 0;
            int64_t offset = 0;

            for (;;) {
                r = archive_read_data_block(a.get(), &buf, &size, &offset);
                LibarchiveException::ThrowIfFailed(r);

                if (size == 0)
                    break;

                totalReadSize += size;
            }
        }
    }
    catch (const LibarchiveException &ex) {
        printf("Caught LibarchiveException. Error: %d\n", ex.GetErrorCode());
    }
    catch (const Win32Exception &ex) {
        printf("Caught Win32Exception. GetLastError(): %d\n", ex.GetLastError());
    }
    catch (...) {
        printf("Caught unknown exception!\n");
    }

    printf("Read: %" PRId64 " of %" PRId64 " discovered bytes", totalReadSize, totalDiscoveredSize);

    return;
}

std::wstring CabMsZipTest::CreateCABFile() {
    // for now just hardcoded string
    // to create file use - makecab "file" "J:\TEMP\archive.cab"
    return LR"(J:\TEMP\archive.cab)";
}