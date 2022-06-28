module;

#include <windows.h>

export module winapi.processthreadsapi;

import <cstdint>;

import winapi.handleapi;
import winapi.errhandlingapi;

export namespace winapi::process
{
    enum class access : std::uint32_t
    {
        del = DELETE,
        read_control = READ_CONTROL,
        synchronize = SYNCHRONIZE,
        write_dac = WRITE_DAC,
        write_owner = WRITE_OWNER,
        all_access = PROCESS_ALL_ACCESS,
        create_process = PROCESS_CREATE_PROCESS,
        create_thread = PROCESS_CREATE_THREAD,
        dup_handle = PROCESS_DUP_HANDLE,
        query_information = PROCESS_QUERY_INFORMATION,
        query_limited_information = PROCESS_QUERY_LIMITED_INFORMATION,
        set_quota = PROCESS_SET_QUOTA,
        suspend_resume = PROCESS_SUSPEND_RESUME,
        terminate = PROCESS_TERMINATE,
        vm_operation = PROCESS_VM_OPERATION,
        vm_read = PROCESS_VM_READ,
        vm_write = PROCESS_VM_WRITE,
    };

    safe_handle open_process(access access, bool inherit, DWORD pid)
    {
        auto h = OpenProcess((unsigned long)access, inherit, pid);
        if (h == INVALID_HANDLE_VALUE)
            throw get_last_error();

        return safe_handle(h);
    }

    safe_handle create_remote_thread(const safe_handle &proc_handle,
                                     void *security_attr,
                                     std::size_t stack_size,
                                     auto *start_routine,
                                     auto *param,
                                     DWORD create_flags,
                                     std::uint32_t *thread_id)
    {
        auto h = CreateRemoteThread(proc_handle.get_unsafe_handle(),
                                    static_cast<LPSECURITY_ATTRIBUTES>(security_attr),
                                    static_cast<SIZE_T>(stack_size),
                                    static_cast<LPTHREAD_START_ROUTINE>(start_routine),
                                    static_cast<LPVOID>(param),
                                    static_cast<DWORD>(create_flags),
                                    reinterpret_cast<LPDWORD>(&thread_id));

        if (h == INVALID_HANDLE_VALUE)
            throw get_last_error();

        return safe_handle(h);
    }
}