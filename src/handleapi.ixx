module;

#include <windows.h>

export module winapi.handleapi;

import winapi.errhandling;

export namespace winapi
{
    export class handle
    {
    public:
        explicit handle(HANDLE h) noexcept : _handle(h) {}

        explicit handle(const handle &h) noexcept
        {
            _handle = h._handle;
        }

        explicit handle(handle &&hndl) noexcept
        {
            _handle = hndl._handle;
            hndl._handle = INVALID_HANDLE_VALUE;
        }

        HANDLE get_raw_handle() const noexcept
        {
            return _handle;
        }

        void close()
        {
            if (_handle != INVALID_HANDLE_VALUE)
                if (CloseHandle(_handle) != 0)
                    throw get_last_error();
        }

        ~handle()
        {
            close();
        }

    private:
        HANDLE _handle;
    };
}