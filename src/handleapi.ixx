module;

#include <windows.h>
#include <handleapi.h>

export module winapi.handleapi;

import <utility>;

import winapi.errhandlingapi;

export namespace winapi
{
    typedef HANDLE unsafe_handle_t;

    export class handle
    {
    public:
        handle() : _handle(INVALID_HANDLE_VALUE) {}

        handle(unsafe_handle_t handle) : _handle(handle) {}

        handle(const handle &other) : _handle(other._handle) {}

        handle(handle &&other) : _handle(other._handle) { other._handle = INVALID_HANDLE_VALUE; }

        handle &operator=(const handle &h)
        {
            _handle = h._handle;
            return *this;
        }

        handle &operator=(handle &&h)
        {
            _handle = h._handle;
            h._handle = INVALID_HANDLE_VALUE;
            return *this;
        }

        unsafe_handle_t get_unsafe_handle() const { return _handle; }

        operator bool() const { return _handle != INVALID_HANDLE_VALUE; }

        virtual ~handle() = default;

    private:
        unsafe_handle_t _handle;
    };

    export class safe_handle : public handle
    {
    public:
        safe_handle() : handle() {}

        safe_handle(unsafe_handle_t handle) : handle(handle) {}

        safe_handle(const safe_handle &other) = delete;

        safe_handle(safe_handle &&other) : handle(std::move(other)) {}

        safe_handle &operator=(const safe_handle &h) = delete;

        safe_handle &operator=(safe_handle &&h)
        {
            handle::operator=(std::move(h));
            return *this;
        }

        operator bool() const { return handle::operator bool(); }

        ~safe_handle() override
        {
            if (*this)
                CloseHandle(get_unsafe_handle());
        }
    };
}