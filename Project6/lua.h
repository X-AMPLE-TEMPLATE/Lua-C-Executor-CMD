#include <Windows.h>
#include <iostream>
#include <string>
#include "global.h"

DWORD ScriptContext;
DWORD RESTATE;

//int x(int Offset) {
//	return (Offset - 0x120000 + (DWORD)GetModuleHandle("RobloxPlayerBeta.exe"));
//}

namespace Memory {

    bool compare(BYTE* address, BYTE* pattern, BYTE* mask) {
        for (; *mask; address++, pattern++, mask++) {
            if (*mask == 'x' && *address != *pattern) {
                return false;
            }
        }
        return true;
    }

    DWORD Scan(BYTE* aob, BYTE* mask, BYTE prot = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)) {
        MEMORY_BASIC_INFORMATION mbi;
        DWORD j = (DWORD)GetModuleHandle(NULL);
        while (j < 0x7FFFFFFF && VirtualQuery((void*)j, &mbi, sizeof(mbi))) {
            if (!(mbi.Protect & PAGE_GUARD) && (mbi.State & MEM_COMMIT) && (mbi.Protect & prot)) {
                for (DWORD k = (DWORD)mbi.BaseAddress; k < ((DWORD)mbi.BaseAddress + mbi.RegionSize); ++k) {
                    if (compare((BYTE*)k, (BYTE*)aob, (BYTE*)mask)) {
                        return k;
                    }
                }
            }
            j += mbi.RegionSize;
        }
        return 0;
    }
}

DWORD unprotect(DWORD addr)
{
    BYTE* tAddr = (BYTE*)addr;
    do
    {
        tAddr += 16;
    } while (!(tAddr[0] == 0x55 && tAddr[1] == 0x8B && tAddr[2] == 0xEC));

    DWORD funcSz = tAddr - (BYTE*)addr;

    PVOID nFunc = VirtualAlloc(NULL, funcSz, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (nFunc == NULL)
        return addr;

    memcpy(nFunc, (void*)addr, funcSz);

    BYTE* pos = (BYTE*)nFunc;
    BOOL valid = false;
    do
    {
        if (pos[0] == 0x72 && pos[2] == 0xA1 && pos[7] == 0x8B) {
            *(BYTE*)pos = 0xEB;

            DWORD cByte = (DWORD)nFunc;
            do
            {
                if (*(BYTE*)cByte == 0xE8)
                {
                    DWORD oFuncPos = addr + (cByte - (DWORD)nFunc);
                    DWORD oFuncAddr = (oFuncPos + *(DWORD*)(oFuncPos + 1)) + 5;

                    if (oFuncAddr % 16 == 0)
                    {
                        DWORD relativeAddr = oFuncAddr - cByte - 5;
                        *(DWORD*)(cByte + 1) = relativeAddr;

                        cByte += 4;
                    }
                }

                cByte += 1;
            } while (cByte - (DWORD)nFunc < funcSz);

            valid = true;
        }
        pos += 1;
    } while ((DWORD)pos < (DWORD)nFunc + funcSz);

    if (!valid)
    {
        VirtualFree(nFunc, funcSz, MEM_RELEASE);
        return addr;
    }

    return (DWORD)nFunc;
} 

// 0x377AE0

////////////////////////////////////////////////////
typedef int(__cdecl* print)(int a1, const char* a2);
print cprint = (print) (unprotect(x(0x377AE0)));
typedef void(__stdcall* rgetfield)(DWORD rL, int idx, const char* k);
rgetfield r_lgfield = (rgetfield) (unprotect(x(0x13411D0)));
typedef void(__fastcall* rpushstring)(DWORD rL, const char*);
rpushstring r_lpstring = (rpushstring) (unprotect(x(0x1341C80)));
typedef int(__cdecl* pcall)(DWORD rL, DWORD, DWORD, DWORD);
pcall r_lpcall = (pcall) (unprotect(x(0x1341980)));
//typedef void(__stdcall* pushvalue)(DWORD rL, DWORD idx);
//pushvalue r_lpvalue = (pushvalue) (unprotect(x(0x5487)));
////////////////////////////////////////////////////


void cnsb() {
	DWORD CheckerValue;
	VirtualProtect((PVOID)&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &CheckerValue);
	*(BYTE*)(&FreeConsole) = (0xC3);
}


void console(const char* csttl) {
	cnsb();
	AllocConsole();
	SetConsoleTitleA(csttl);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::ShowWindow(ConsoleHandle, SW_NORMAL);
}