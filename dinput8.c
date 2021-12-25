#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>

typedef HRESULT (WINAPI *dinp8crt_t)(HINSTANCE, DWORD, REFIID, LPVOID *, LPUNKNOWN);
dinp8crt_t oDirectInput8Create;

__attribute__ ((dllexport))
HRESULT WINAPI DirectInput8Create(HINSTANCE inst, DWORD ver, REFIID id, LPVOID *pout, LPUNKNOWN outer)
{
    return oDirectInput8Create(inst, ver, id, pout, outer);
}

void setup_d8proxy(void)
{
    char syspath[320];
    GetSystemDirectoryA(syspath, 320);
    strcat(syspath, "\\dinput8.dll");
    HMODULE mod = LoadLibraryA(syspath);
    oDirectInput8Create = (dinp8crt_t)GetProcAddress(mod, "DirectInput8Create");
}

// load from given directory DLLs whose name contains given string
void loadDLLs(char *dirName, char *flt)
{
char *tgt;
struct dirent *dp;
DIR *dir0 = opendir(dirName);
if (!dir0) return;
if (strcmp(dirName,".") != 0 ) SetDllDirectory(dirName);
while( dp = readdir(dir0) )
 {
 tgt = dp->d_name; 
 if (!strstr(tgt, ".dll") || !strstr(tgt, flt) ) continue;
 LoadLibraryA(tgt);
 }
} 

void attach_hook(void)
{
loadDLLs("_load_",".dll");
loadDLLs(".","_load_");
}



BOOL APIENTRY DllMain(HMODULE mod, DWORD reason,
		      LPVOID res)
{
    switch (reason) {
    case DLL_PROCESS_ATTACH:
	setup_d8proxy();
	attach_hook();
	break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
	break;
    }
    return TRUE;
}
