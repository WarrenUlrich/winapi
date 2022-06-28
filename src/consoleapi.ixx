module;

#include <windows.h>

export module winapi.consoleapi;

import winapi.errhandlingapi;

export namespace winapi::console
{
    void alloc_console()
    {
        if (AllocConsole() == 0)
            throw get_last_error();
    }
}