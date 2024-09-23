// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <tchar.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DWORD currentProcessId = GetCurrentProcessId();
        TCHAR processName[MAX_PATH];
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        PROCESSENTRY32 processEntry;
        processEntry.dwSize = sizeof(PROCESSENTRY32);

        if (Process32First(snapshot, &processEntry))
        {
            do
            {
                if (processEntry.th32ProcessID == currentProcessId)
                {
                    _tcsncpy_s(processName, processEntry.szExeFile, MAX_PATH - 1);
                    break;
                }
            } while (Process32Next(snapshot, &processEntry));
        }

        CloseHandle(snapshot);

        TCHAR message[1024];
        _stprintf_s(message, _T("进程名: %s\n进程 ID: %d"), processName, currentProcessId);

        MessageBox(NULL, message, _T("信息"), MB_OK);
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

