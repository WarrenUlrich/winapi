module;

#include <windows.h>
#include <synchapi.h>

export module winapi.synchapi;

import <cstdint>;

import winapi.handleapi;
import winapi.errhandlingapi;

export namespace winapi::sync
{
    enum class wait_result : std::uint32_t
    {
        abandoned = WAIT_ABANDONED,
        signal = WAIT_OBJECT_0,
        timeout = WAIT_TIMEOUT,
        failed = WAIT_FAILED,
    };

    wait_result wait_for_single_object(const safe_handle &handle, std::uint32_t milliseconds)
    {
        auto result = WaitForSingleObject(handle.get_unsafe_handle(), milliseconds);
        if (result == WAIT_FAILED)
            throw get_last_error();

        return static_cast<wait_result>(result);
    }
}