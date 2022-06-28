module;

#include <windows.h>
#include <synchapi.h>

export module winapi.synchapi;

import <cstdint>;
import <chrono>;

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

    template <typename Rep, typename Period>
    wait_result wait_for_single_object(const safe_handle &handle, std::chrono::duration<Rep, Period> duration)
    {
        auto result = WaitForSingleObject(handle.get_unsafe_handle(), std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
        if (result == WAIT_FAILED)
            throw get_last_error();

        return static_cast<wait_result>(result);
    }
}