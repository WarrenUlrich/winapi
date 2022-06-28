module;

#include <windows.h>
#include <handleapi.h>

export module winapi.handleapi;

import winapi.errhandlingapi;

export namespace winapi
{
    typedef HANDLE unsafe_handle_t;

    export class safe_handle
    {
    public:
        safe_handle() : _handle(INVALID_HANDLE_VALUE) {}

        safe_handle(unsafe_handle_t h) noexcept : _handle(h) {}

        safe_handle(const safe_handle &) = delete;

        safe_handle(safe_handle &&hndl) noexcept
        {
            _handle = hndl._handle;
            hndl._handle = INVALID_HANDLE_VALUE;
        }

        safe_handle &operator=(const safe_handle &) = delete;

        safe_handle &operator=(safe_handle &&h)
        {
            _handle = h._handle;
            h._handle = INVALID_HANDLE_VALUE;
            return *this;
        }

        unsafe_handle_t get_unsafe_handle() const noexcept
        {
            return _handle;
        }

        bool valid() const noexcept
        {
            return _handle != INVALID_HANDLE_VALUE;
        }

        void close()
        {
            if (valid())
                if (CloseHandle(_handle) == 0)
                    throw get_last_error();
        }

        virtual ~safe_handle()
        {
            close();
        }

    protected:
        unsafe_handle_t _handle;
    };
}