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
    enum class alloc_type : std::uint32_t
    {
        commit = MEM_COMMIT,
        reserve = MEM_RESERVE,
        reset = MEM_RESET,
        undo = MEM_RESET_UNDO,
        large_pages = MEM_LARGE_PAGES,
        physical = MEM_PHYSICAL,
        top_down = MEM_TOP_DOWN,
    };

    enum class free_type : std::uint32_t
    {
        decommit = MEM_DECOMMIT,
        release = MEM_RELEASE,
    };

    enum class protect : std::uint32_t
    {
        no_access = PAGE_NOACCESS,
        guard = PAGE_GUARD,
        no_cache = PAGE_NOCACHE,
        write_combine = PAGE_WRITECOMBINE,
        read_write = PAGE_READWRITE,
        execute_read_write = PAGE_EXECUTE_READWRITE,
    };

    void write_process_memory(const safe_handle &h, std::uintptr_t base_addr, auto *buffer, std::size_t size, std::size_t *bytes_written)
    {
        if (WriteProcessMemory(h.get_unsafe_handle(), (void *)base_addr, (void *)buffer, (SIZE_T)size, (SIZE_T *)bytes_written) == 0)
        {
            throw get_last_error();
        }
    }

    template <typename T>
    void write_process_memory(const safe_handle &h, std::uintptr_t base_addr, const T &value)
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

    std::uintptr_t virtual_alloc_ex(const safe_handle &h, std::uintptr_t addr, std::size_t size, alloc_type flags, protect protect)
    {
        auto result = VirtualAllocEx(h.get_unsafe_handle(), reinterpret_cast<void *>(addr), static_cast<SIZE_T>(size), static_cast<DWORD>(flags), static_cast<DWORD>(protect));
        if (result == NULL)
        {
            throw get_last_error();
        }

        return reinterpret_cast<std::uintptr_t>(result);
    }

    void virtual_free_ex(const safe_handle &h, std::uintptr_t addr, std::size_t size, free_type flags)
    {
        if (VirtualFreeEx(h.get_unsafe_handle(), reinterpret_cast<void *>(addr), static_cast<SIZE_T>(size), static_cast<DWORD>(flags)) == 0)
        {
            throw get_last_error();
        }
    }
}