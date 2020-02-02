// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#include "HooksManager.h"

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    static HooksManager* manager = new HooksManager();

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        //MessageBox(NULL, L"DLL_PROCESS_ATTACH", L"DLL_PROCESS_ATTACH", MB_ICONINFORMATION);
        break;
    case DLL_THREAD_ATTACH:
        //MessageBox(NULL, L"DLL_THREAD_ATTACH", L"DLL_THREAD_ATTACH", MB_ICONINFORMATION);
        break;
    case DLL_THREAD_DETACH:
        //MessageBox(NULL, L"DLL_THREAD_DETACH", L"DLL_THREAD_DETACH", MB_ICONINFORMATION);
        break;
    case DLL_PROCESS_DETACH:
        //MessageBox(NULL, L"DLL_PROCESS_DETACH", L"DLL_PROCESS_DETACH", MB_ICONINFORMATION);
        break;
    }
    return TRUE;
}
