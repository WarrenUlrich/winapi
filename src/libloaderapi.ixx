module;

#include <windows.h>

export module winapi.libloaderapi;

import <string_view>;

import winapi.handleapi;
import winapi.errhandlingapi;

export namespace winapi::lib
{
    safe_handle load_library(std::string_view file_name)
    {
        auto h = LoadLibraryA(file_name.data());
        if (h == INVALID_HANDLE_VALUE)
            throw get_last_error();

        return safe_handle(h);
    }

    safe_handle load_library(std::wstring_view file_name)
    {
        auto h = LoadLibraryW(file_name.data());
        if (h == INVALID_HANDLE_VALUE)
            throw get_last_error();

        return safe_handle(h);
    }

    template<typename T>
    T* get_proc_address(const safe_handle& lib, std::string_view proc_name)
    {
        auto proc = GetProcAddress((HMODULE)lib.get_unsafe_handle(), proc_name.data());
        if (proc == nullptr)
            throw get_last_error();
        
        return (T*)proc;
    }
}