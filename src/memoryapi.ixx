module;

#include <windows.h>

export module winapi.memoryapi;

import <cstddef>;
import <cstdint>;
import <stdexcept>;

import winapi.handleapi;
import winapi.errhandlingapi;

export namespace winapi::memory
{
    void write_process_memory(const safe_handle& h, std::uintptr_t base_addr, auto* buffer, std::size_t size, std::size_t* bytes_written)
    {
        if (WriteProcessMemory(h.get_unsafe_handle(), (void*)base_addr, (void*) buffer, (SIZE_T)size, (SIZE_T*)bytes_written) == 0)
        {
            throw get_last_error();
        }
    }

    template<typename T>
    void write_process_memory(const safe_handle& h, std::uintptr_t base_addr, const T& value)
    {
        constexpr auto size = sizeof(T);
        write_process_memory(h, base_addr, &value, size, nullptr);
    }

    void read_process_memory(const safe_handle &h, std::uintptr_t base_addr, auto *buffer, std::size_t size, std::size_t *bytes_read)
    {
        if (ReadProcessMemory(h.get_unsafe_handle(), (void *)base_addr, (void *)buffer, (SIZE_T)size, (SIZE_T *)bytes_read) == 0)
        {
            throw get_last_error();
        }
    }

    template <typename T>
    T read_process_memory(const safe_handle &h, std::uintptr_t base_addr)
    {
        constexpr auto size = sizeof(T);
        T result{};

        read_process_memory(h, base_addr, &result, size, nullptr);
        return result;
    }
}