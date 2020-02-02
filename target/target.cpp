// target.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

/* Note: could also use malloc() and free() */
void printAdapterInfo();

int __cdecl main()
{
    while (true) {
        printAdapterInfo();
        _getch();
    }
   
    return 0;
}

// This is MSDN example: https://docs.microsoft.com/en-us/windows/win32/api/iphlpapi/nf-iphlpapi-getadaptersinfo?redirectedfrom=MSDN
void printAdapterInfo() {
    /* Declare and initialize variables */

    // It is possible for an adapter to have multiple
    // IPv4 addresses, gateways, and secondary WINS servers
    // assigned to the adapter. 
    //
    // Note that this sample code only prints out the 
    // first entry for the IP address/mask, and gateway, and
    // the primary and secondary WINS server for each adapter. 

    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;
    UINT i;
    
    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(sizeof(IP_ADAPTER_INFO));
    if (pAdapterInfo == NULL) {
        printf("Error allocating memory needed to call GetAdaptersinfo\n");
        return;
    }
    // Make an initial call to GetAdaptersInfo to get
    // the necessary size into the ulOutBufLen variable
    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        FREE(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(ulOutBufLen);
        if (pAdapterInfo == NULL) {
            printf("Error allocating memory needed to call GetAdaptersinfo\n");
            return;
        }
    }

    if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
        pAdapter = pAdapterInfo;
        while (pAdapter) {
            printf("\tComboIndex: \t%d\n", pAdapter->ComboIndex);
            printf("\tAdapter Name: \t%s\n", pAdapter->AdapterName);
            printf("\tAdapter Desc: \t%s\n", pAdapter->Description);
            printf("\tAdapter Addr: \t");
            for (i = 0; i < pAdapter->AddressLength; i++) {
                if (i == (pAdapter->AddressLength - 1))
                    printf("%.2X\n", (int)pAdapter->Address[i]);
                else
                    printf("%.2X-", (int)pAdapter->Address[i]);
            }
            printf("\tIndex: \t%d\n", pAdapter->Index);
            
            pAdapter = pAdapter->Next;
            printf("\n");
        }
    }
    else {
        printf("GetAdaptersInfo failed with error: %d\n", dwRetVal);

    }
    if (pAdapterInfo)
        FREE(pAdapterInfo);

}