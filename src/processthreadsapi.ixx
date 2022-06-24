module;

#include <windows.h>

export module winapi.processthreadsapi;

import winapi.handleapi;
import winapi.errhandlingapi;

export namespace winapi::process
{
    enum class access
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
}