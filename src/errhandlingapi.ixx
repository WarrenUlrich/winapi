module;

#include <windows.h>

export module winapi.errhandlingapi;

import <system_error>;

export namespace winapi
{
    std::system_error get_last_error()
    {
        return std::system_error(std::error_code(GetLastError(), std::system_category()));
    }
}