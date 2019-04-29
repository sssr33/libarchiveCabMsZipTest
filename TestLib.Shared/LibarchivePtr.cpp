#include "LibarchivePtr.h"

void LibarchiveReadDeleter::operator()(archive *arch) {
    archive_read_free(arch);
}