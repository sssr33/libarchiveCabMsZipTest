#pragma once

#include <memory>
#include <libarchive/archive.h>

struct LibarchiveReadDeleter {
    void operator()(archive *arch);
};

typedef std::unique_ptr<archive, LibarchiveReadDeleter> ULibarchiveReadPtr;