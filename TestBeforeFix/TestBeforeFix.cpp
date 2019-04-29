// TestBeforeFix.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include <iostream>
#include <conio.h>
#include <Test.h>

int main() {
    Test::RunAllTest();

    printf("\n\npress any key to exit...");
    _getch();
    return 0;
}