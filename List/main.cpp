#include <stdio.h>
#include <Windows.h>

#include "List\ListUnitTests.h"

#include "Logs/Logs.h"

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    LogsConstructor();

    ListUnitTest_Resize1();

    LogsDestructor();
}