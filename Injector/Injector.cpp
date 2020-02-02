// Injector.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

void PrintError(LPTSTR lpszFunction);
BOOL WINAPI InjectDll(__in LPCWSTR lpcwszDll, __in LPCWSTR targetPath);

int main()
{
    wchar_t selfdir[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, selfdir, MAX_PATH);
    PathRemoveFileSpec(selfdir);

    std::wstring dllPath = std::wstring(selfdir) + TEXT("\\hooks.dll");
    std::wstring targetPath = std::wstring(selfdir) + TEXT("\\target.exe");

    if (InjectDll(dllPath.c_str(), targetPath.c_str())) {
        printf("Dll was successfully injected.\n");
    }
    else {
        printf("Terminating the Injector app...");
    }

    _getch();

    return 0;
}


BOOL WINAPI InjectDll(__in LPCWSTR lpcwszDll, __in LPCWSTR targetPath)
{
    SIZE_T nLength;
    LPVOID lpLoadLibraryW = NULL;
    LPVOID lpRemoteString;
    STARTUPINFO             startupInfo;
    PROCESS_INFORMATION     processInformation;

    memset(&startupInfo, 0, sizeof(startupInfo));
    startupInfo.cb = sizeof(STARTUPINFO);

    if (!CreateProcess(targetPath, NULL, NULL, NULL, FALSE,
        CREATE_SUSPENDED, NULL, NULL, &startupInfo, &processInformation))
    {
        PrintError(TEXT("CreateProcess"));
        return FALSE;
    }

    lpLoadLibraryW = GetProcAddress(GetModuleHandle(L"KERNEL32.DLL"), "LoadLibraryW");

    if (!lpLoadLibraryW)
    {
        PrintError(TEXT("GetProcAddress"));
        // close process handle
        CloseHandle( processInformation.hProcess);
        return FALSE;
    }

    nLength = wcslen(lpcwszDll) * sizeof(WCHAR);

    // allocate mem for dll name
    lpRemoteString = VirtualAllocEx(processInformation.hProcess, NULL, nLength + 1, MEM_COMMIT, PAGE_READWRITE);
    if (!lpRemoteString)
    {
        PrintError(TEXT("VirtualAllocEx"));

        // close process handle
        CloseHandle(processInformation.hProcess);

        return FALSE;
    }

    // write dll name
    if (!WriteProcessMemory(processInformation.hProcess, lpRemoteString, lpcwszDll, nLength, NULL)) {

        PrintError(TEXT("WriteProcessMemory"));
        // free allocated memory
        VirtualFreeEx(processInformation.hProcess, lpRemoteString, 0, MEM_RELEASE);

        // close process handle
        CloseHandle(processInformation.hProcess);

        return FALSE;
    }

    // call loadlibraryw
    HANDLE hThread = CreateRemoteThread(processInformation.hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)lpLoadLibraryW, lpRemoteString, NULL, NULL);

    if (!hThread) {
        PrintError(TEXT("CreateRemoteThread"));
    }
    else {
        WaitForSingleObject(hThread, 4000);

        //resume suspended process
        ResumeThread(processInformation.hThread);
    }

    //  free allocated memory
    VirtualFreeEx(processInformation.hProcess, lpRemoteString, 0, MEM_RELEASE);

    // close process handle
    CloseHandle(processInformation.hProcess);

    return TRUE;
}

void PrintError(LPTSTR lpszFunction)
{
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
        (LPTSTR)&lpMsgBuf,
        0, NULL);

    // Display the error message

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf,
        LocalSize(lpDisplayBuf) / sizeof(TCHAR),
        TEXT("%s failed with error %d: %s"),
        lpszFunction, dw, lpMsgBuf);

    wprintf(L"%s", lpDisplayBuf);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}