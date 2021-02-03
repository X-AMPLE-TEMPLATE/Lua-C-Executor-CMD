#include <Windows.h>
#include <iostream>
#include <string>
#include "lua.h"
#include "global.h"


int main() { return 0; }


using namespace std;

// 0 = Print, 1 = Info, 2 = Warn, 3 = Error

void mainlua() {

	auto SCVFTBLE = unprotect(x(0x123B6F0));
	console("Lua C Executor");
	cprint(1,"Lua C Executor loaded");
	cout << "Started scanning...... " << "\n";
	ScriptContext = Memory::Scan((BYTE*)&SCVFTBLE, (BYTE*)"xxxx", PAGE_READWRITE);
	if (!ScriptContext) {
		cprint(3,"ScriptContext Scan Failed!\n");
		system("pause");
		exit(0);
	}

	//int v39 = ScriptContext;
	//int v51 = 0;
	RESTATE = (ScriptContext + 56 * 0 + 172) ^ *(DWORD*)(ScriptContext + 56 * 1 + 172);
	cprint(0,"Finished the scanning!\n");

	cout << "RSTATE:  " << uppercase << hex << ScriptContext << endl;
	cout << "LUA STATE:  " << uppercase << hex << RESTATE << endl;

	do {
		//string numstr;
		string outstr;
		string input;
		//int number = 0;
		getline(cin, outstr);
		getline(cin, input);
		if (outstr == "print")
		{
			cout << "Printed: " + outstr;
			r_l_getglobal(RESTATE, "game");
			r_lpstring(RESTATE, input.c_str());
			r_lpcall(RESTATE, 1, 0, 0);
		};
		//getline(cin, numstr);
		//number = stoi(numstr);
		//SingletonPrint(PrintExploit::Integers::printtype, outstr.c_str());
		//cout << "Printed " << "Type:" << number << " Text:" << outstr << " Successfully! \n" ;
	} while (true);

}


int __stdcall DllMain(HMODULE DLL, DWORD Reason, void* Useless) {
	if (Reason == 1) {
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)mainlua, 0, 0, 0);
	}
	return TRUE;
}