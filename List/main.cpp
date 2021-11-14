#include <stdio.h>
#include <Windows.h>

#include "List.h"


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    List lst = {};

    const size_t cap = 13;
    ListConstructor(&lst, cap);

    ListAddElemAfter(&lst, 10, 0);
    ListAddElemAfter(&lst, 20, 2);
    ListAddElemAfter(&lst, 30, 7);
    ListAddElemAfter(&lst, 40, 3);
    ListAddElemAfter(&lst, 50, 10);
    ListAddElemAfter(&lst, 60, 12);

    ListSort(&lst);
    
    ListDump(&lst, stdout);
}