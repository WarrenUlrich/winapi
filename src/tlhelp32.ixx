module;

#include <windows.h>
#include <TlHelp32.h>

export module winapi.tlhelp32;

import <utility>;
import <iterator>;
import <string_view>;
import <stdexcept>;

import winapi.handleapi;

export namespace winapi::tool_help
{
    using raw_process_entry32 = PROCESSENTRY32;

    class process_entry32
    {
    public:
        process_entry32() : _entry()
        {
            _entry.dwSize = sizeof(raw_process_entry32);
        }

        process_entry32(const raw_process_entry32 &entry) : _entry(entry)
        {
            _entry.dwSize = sizeof(raw_process_entry32);
        }

        process_entry32 &operator=(const raw_process_entry32 &entry)
        {
            _entry = entry;
            return *this;
        }

        process_entry32 &operator=(raw_process_entry32 &&entry)
        {
            _entry = std::move(entry);
            return *this;
        }

        process_entry32(raw_process_entry32 &&entry) : _entry(std::move(entry)) {}

        raw_process_entry32 &get_raw_process_entry32()
        {
            return _entry;
        }

        std::uint32_t size() const noexcept
        {
            return _entry.dwSize;
        }

        std::uint32_t usage() const noexcept
        {
            return _entry.cntUsage;
        }

        std::uint32_t pid() const noexcept
        {
            return _entry.th32ProcessID;
        }

        std::uintptr_t default_heap_id() const noexcept
        {
            return _entry.th32DefaultHeapID;
        }

        std::uint32_t thread_count() const noexcept
        {
            return _entry.cntThreads;
        }

        std::uint32_t parent_pid() const noexcept
        {
            return _entry.th32ParentProcessID;
        }

        long priority_class_base() const noexcept
        {
            return _entry.pcPriClassBase;
        }

        std::uint32_t flags() const noexcept
        {
            return _entry.dwFlags;
        }

        std::string_view file_name() const
        {
            for (auto idx = 0; idx < sizeof(_entry.szExeFile); idx++)
            {
                if (_entry.szExeFile[idx] == '\0')
                    return std::string_view(_entry.szExeFile, idx);
            }

            throw std::runtime_error("unterminated file_name string");
        }

    private:
        raw_process_entry32 _entry;
    };

    class process_entry32_iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = process_entry32;
        using pointer = process_entry32 *;
        using reference = process_entry32 &;

        process_entry32_iterator(safe_handle &&snapshot) : _snapshot(std::move(snapshot)), _done(false)
        {
            _done = Process32First(_snapshot.get_unsafe_handle(), &_current.get_raw_process_entry32()) == 0;
        }

        bool operator==(const std::default_sentinel_t &) const noexcept
        {
            return _done;
        }

        bool operator!=(const std::default_sentinel_t &s) const noexcept
        {
            return !(*this == s);
        }

        process_entry32_iterator &operator++() noexcept
        {
            _done = Process32Next(_snapshot.get_unsafe_handle(), &_current.get_raw_process_entry32()) == 0;
            return *this;
        }

        reference operator*() noexcept
        {
            return _current;
        }

        pointer operator->() noexcept
        {
            return &_current;
        }

    private:
        safe_handle _snapshot;
        bool _done;
        process_entry32 _current;
    };

    class process_entry32_range
    {
    public:
        using iterator = process_entry32_iterator;

        process_entry32_range(safe_handle &&snapshot) : _snapshot(std::move(snapshot)) {}

        iterator begin() noexcept
        {
            return iterator(std::move(_snapshot));
        }

        std::default_sentinel_t end() noexcept
        {
            return {};
        }

        static process_entry32_range all()
        {
            auto unsafe_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            return process_entry32_range(safe_handle(unsafe_handle));
        }

    private:
        safe_handle _snapshot;
    };

    using raw_module_entry32 = MODULEENTRY32;

    class module_entry32
    {
    public:
        module_entry32() : _entry()
        {
            _entry.dwSize = sizeof(raw_module_entry32);
        }

        module_entry32(const raw_module_entry32 &entry) : _entry(entry) {}

        module_entry32(raw_module_entry32 &&entry) : _entry(std::move(entry)) {}

        module_entry32(const module_entry32 &entry) : _entry(entry._entry) {}

        module_entry32(module_entry32 &&entry) : _entry(std::move(entry._entry)) {}

        module_entry32 &operator=(const module_entry32 &entry)
        {
            _entry = entry._entry;
            return *this;
        }

        module_entry32 &operator=(module_entry32 &&entry)
        {
            _entry = std::move(entry._entry);
            return *this;
        }

        raw_module_entry32 &get_raw_module_entry32() noexcept
        {
            return _entry;
        }

        std::uint32_t size() const noexcept
        {
            return _entry.dwSize;
        }

        std::uint32_t module_id() const noexcept
        {
            return _entry.th32ModuleID;
        }

        std::uint32_t pid() const noexcept
        {
            return _entry.th32ProcessID;
        }

        std::uint32_t global_usage_count() const noexcept
        {
            return _entry.GlblcntUsage;
        }

        std::uint32_t local_usage_count() const noexcept
        {
            return _entry.ProccntUsage;
        }

        std::uintptr_t base_addr() const noexcept
        {
            return reinterpret_cast<std::uintptr_t>(_entry.modBaseAddr);
        }

        std::uint32_t base_size() const noexcept
        {
            return _entry.modBaseSize;
        }

        unsafe_handle_t module_handle() const noexcept
        {
            return _entry.hModule;
        }

        std::string_view module_name() const noexcept
        {
            for (auto idx = 0; idx < sizeof(_entry.szModule); idx++)
            {
                if (_entry.szModule[idx] == '\0')
                    return std::string_view(_entry.szModule, idx);
            }

            throw std::runtime_error("unterminated module_name string");
        }

        std::string_view exe_path() const noexcept
        {
            for (auto idx = 0; idx < sizeof(_entry.szExePath); idx++)
            {
                if (_entry.szExePath[idx] == '\0')
                    return std::string_view(_entry.szExePath, idx);
            }

            throw std::runtime_error("unterminated exe_path string");
        }

    private:
        raw_module_entry32 _entry;
    };

    class module_entry32_iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = module_entry32;
        using pointer = value_type *;
        using reference = value_type &;

        module_entry32_iterator(safe_handle &&snapshot) : _snapshot(std::move(snapshot)), _done(false), _current()
        {
            _done = Module32First(_snapshot.get_unsafe_handle(), &_current.get_raw_module_entry32()) == 0;
        }

        bool operator==(const std::default_sentinel_t &) const noexcept
        {
            return _done;
        }

        bool operator!=(const std::default_sentinel_t &s) const noexcept
        {
            return !(*this == s);
        }

        module_entry32_iterator &operator++() noexcept
        {
            _done = Module32Next(_snapshot.get_unsafe_handle(), &_current.get_raw_module_entry32()) == 0;
            return *this;
        }

        reference operator*() noexcept
        {
            return _current;
        }

        pointer operator->() noexcept
        {
            return &_current;
        }

    private:
        safe_handle _snapshot;
        bool _done;
        module_entry32 _current;
    };

    class module_entry32_range
    {
    public:
        using iterator = module_entry32_iterator;

        module_entry32_range(safe_handle &&snapshot) : _snapshot(std::move(snapshot)) {}

        iterator begin() noexcept
        {
            return iterator(std::move(_snapshot));
        }

        std::default_sentinel_t end() noexcept
        {
            return {};
        }

        static module_entry32_range all(std::uint32_t pid)
        {
            auto unsafe_handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
            return module_entry32_range(safe_handle(unsafe_handle));
        }

    private:
        safe_handle _snapshot;
    };
}