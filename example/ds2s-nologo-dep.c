#include <windows.h>

void main1()
{
void *base_addr = GetModuleHandle(NULL);
*(byte*)(base_addr + 0x1604DFA) = 0x01;
}
	
BOOL APIENTRY DllMain(HMODULE mod, DWORD reason,
		      LPVOID res)
{                  
    main1();
    return TRUE;
}
