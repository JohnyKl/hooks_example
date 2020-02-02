#pragma once
#include "stdafx.h"

class HooksManager
{
    static bool isInitialized;
    static BOOL(__cdecl *HookFunction)(ULONG_PTR OriginalFunction, ULONG_PTR NewFunction);
    static VOID(__cdecl *UnhookFunction)(ULONG_PTR Function);

    HMODULE hLibrary = NULL; 
public:
    HooksManager();
    ~HooksManager();

    static ULONG_PTR(__cdecl *GetOriginalFunction)(ULONG_PTR Hook);

    void hookFunctions();
    void removeHooks();
};

