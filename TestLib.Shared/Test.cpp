#include "Test.h"
#include "CabMsZipTest.h"

void Test::RunAllTest() {
    {
        CabMsZipTest test;
        test.Run();
    }
}