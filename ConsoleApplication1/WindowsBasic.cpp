#include "PreComplier.h"
#include "WindowsBasic.h"
#include <windows.h>
#include <iostream>
#include <TlHelp32.h>

namespace PraticeWin
{
    // 要注入的 DLL 路径
    const WCHAR* dllPath = L"C:\\Users\\15380\\source\\repos\\ConsoleApplication1\\x64\\Debug\\Dll1.dll";

    // 远程线程函数的定义
    typedef HMODULE(WINAPI* pLoadLibraryA)(LPCSTR);

    // 定义函数指针类型，与 DLL 中的函数原型匹配
    typedef int(*MYFUNC)(const int&);

    static void PrintError()
    {
        DWORD error = GetLastError();

        // 将错误码转换为消息字符串
        LPWSTR message = nullptr;
        size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&message), 0, nullptr);

        if (size > 0)
        {
            std::wcout << L"错误信息: " << message << std::endl;
            LocalFree(message);
        }
        else
        {
            std::wcout << L"错误码: " << error << std::endl;
        }
    }

    void loadLibFunction()
    {
        HMODULE moudle = LoadLibrary(dllPath);
        if (moudle == NULL)
        {
            std::cerr << "加载 DLL 失败。" << std::endl;
            return;
        }

        // 获取函数地址
        MYFUNC addFunc = (MYFUNC)GetProcAddress(moudle, "fnMyDll");
        if (addFunc == NULL)
        {
            std::cerr << "获取函数地址失败。" << std::endl;
            FreeLibrary(moudle);
            return;
        }

        // 调用函数
        int result = addFunc(3);
        std::cout << "调用 DLL 中的函数结果：" << result << std::endl;

        // 卸载 DLL
        FreeLibrary(moudle);
    }
    // 根据进程名获取进程 ID
    DWORD GetProcessIdByName(const WCHAR* processName)
    {
        PROCESSENTRY32 processEntry;
        processEntry.dwSize = sizeof(PROCESSENTRY32);

        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if (snapshot == INVALID_HANDLE_VALUE)
            return 0;

        if (Process32First(snapshot, &processEntry))
        {
            do
            {
                if (wcscmp(processEntry.szExeFile, processName) == 0)
                {
                    CloseHandle(snapshot);
                    return processEntry.th32ProcessID;
                }
            } while (Process32Next(snapshot, &processEntry));
        }

        CloseHandle(snapshot);
        return 0;
    }

    // 提升权限函数
    BOOL EnableDebugPrivilege()
    {
        HANDLE hToken;
        LUID luid;
        TOKEN_PRIVILEGES tkp;

        if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        {
            std::wcout << L"打开进程令牌失败" << std::endl;
            return FALSE;
        }

        if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
        {
            std::wcout << L"查找权限值失败" << std::endl;
            CloseHandle(hToken);
            return FALSE;
        }

        tkp.PrivilegeCount = 1;
        tkp.Privileges[0].Luid = luid;
        tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

        if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL))
        {
            std::wcout << L"调整权限失败" << std::endl;
            CloseHandle(hToken);
            return FALSE;
        }

        CloseHandle(hToken);
        return TRUE;
    }

    int InjectRemoteThread()
    {

        if (!EnableDebugPrivilege())
        {
            return 1;
        }

        // 找到 explorer.exe 的进程 ID
        DWORD explorerProcessId = GetProcessIdByName(L"ConsoleApplication_test.exe");

        if (explorerProcessId == 0)
        {
            std::wcout << L"找不到 explorer.exe 进程。" << std::endl;
            return 1;
        }

        // 打开目标进程
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, explorerProcessId);

        if (hProcess == NULL)
        {
            std::wcout << L"打开目标进程失败。" << std::endl;
            return 1;
        }

        // 在目标进程中分配内存，用于存储 DLL 路径
        LPVOID remoteDllPath = VirtualAllocEx(hProcess, NULL, (wcslen(dllPath) + 1) * sizeof(WCHAR), MEM_COMMIT, PAGE_EXECUTE_READWRITE);

        if (remoteDllPath == NULL)
        {
            std::wcout << L"在目标进程中分配内存失败。" << std::endl;
            CloseHandle(hProcess);
            return 1;
        }

        // 将 DLL 路径写入目标进程的内存空间
        if (!WriteProcessMemory(hProcess, remoteDllPath, dllPath, (wcslen(dllPath) + 1) * sizeof(WCHAR), NULL))
        {
            std::wcout << L"写入目标进程内存失败。" << std::endl;
            VirtualFreeEx(hProcess, remoteDllPath, 0, MEM_RELEASE);
            CloseHandle(hProcess);
            return 1;
        }

        // 获取 LoadLibraryA 函数的地址
        pLoadLibraryA loadLibraryAddress = (pLoadLibraryA)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW");

        if (loadLibraryAddress == NULL)
        {
            std::wcout << L"获取 LoadLibraryA 函数地址失败。" << std::endl;
            VirtualFreeEx(hProcess, remoteDllPath, 0, MEM_RELEASE);
            CloseHandle(hProcess);
            return 1;
        }

        // 在目标进程中创建远程线程，执行 LoadLibraryA 函数加载 DLL
        HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibraryAddress, remoteDllPath, 0, NULL);

        if (hRemoteThread == NULL)
        {


            VirtualFreeEx(hProcess, remoteDllPath, 0, MEM_RELEASE);
            CloseHandle(hProcess);
            return 1;
        }

        // 等待远程线程执行完毕
        WaitForSingleObject(hRemoteThread, INFINITE);

        // 释放目标进程中的内存
        VirtualFreeEx(hProcess, remoteDllPath, 0, MEM_RELEASE);
        // 关闭句柄
        CloseHandle(hRemoteThread);
        CloseHandle(hProcess);

        return 0;
    }
}