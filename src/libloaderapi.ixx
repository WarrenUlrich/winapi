module;

#include <windows.h>

export module winapi.libloaderapi;

import <string_view>;

import winapi.handleapi;
import winapi.errhandlingapi;

export namespace winapi::lib
{
    class module_handle : public safe_handle
    {
    public:
        module_handle(unsafe_handle_t h) noexcept : safe_handle(h) {}

        module_handle(const module_handle &) = delete;

        module_handle(module_handle &&hndl) noexcept : safe_handle(std::move(hndl)) {}

        module_handle &operator=(const module_handle &) = delete;

        module_handle &operator=(module_handle &&h)
        {
            safe_handle::operator=(std::move(h));
            return *this;
        }

        template <typename T = void>
        T *get_proc_address(std::string_view proc_name)
        {
            auto proc = GetProcAddress((HMODULE)_handle, proc_name.data());
            if (proc == nullptr)
                throw get_last_error();

            return reinterpret_cast<T *>(proc);
        }

        ~module_handle() override {}
    };

    module_handle load_library(std::string_view file_name)
    {
        auto h = LoadLibraryA(file_name.data());
        if (h == nullptr)
            throw get_last_error();

        return module_handle(h);
    }

    module_handle load_library(std::wstring_view file_name)
    {
        auto h = LoadLibraryW(file_name.data());
        if (h == nullptr)
            throw get_last_error();

        return module_handle(h);
    }

    module_handle get_module_handle(std::string_view module_name)
    {
        auto h = GetModuleHandleA(module_name.data());
        if (h == nullptr)
            throw get_last_error();

        return module_handle(h);
    }
}