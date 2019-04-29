#pragma once
#include "ITest.h"

#include <string>
#include <libarchive/archive.h>
#include <libarchive/archive_entry.h>

class CabMsZipTest : public ITest {
public:
    void Run() override;

private:
    static std::wstring CreateCABFile();
};