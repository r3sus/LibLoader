#include <windows.h>

void main1()
{
*(BYTE*)0x141604DFA = 0x01;
}
	
BOOL APIENTRY DllMain(HMODULE mod, DWORD reason,
		      LPVOID res)
{                  
    main1();
    return TRUE;
}
