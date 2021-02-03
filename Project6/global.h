#pragma once

#define x(x) (x - 0x120000 + (DWORD)GetModuleHandle("RobloxPlayerBeta.exe"))
#define r_l_getglobal(rL,s)	r_lgfield(rL, -10002, (s))

#define R_LUA_TNIL 0
#define R_LUA_TLIGHTUSERDATA 1
#define R_LUA_TNUMBER 2
#define R_LUA_TBOOLEAN 3
#define R_LUA_TSTRING 4
#define R_LUA_TTHREAD 5
#define R_LUA_TFUNCTION 6
#define R_LUA_TTABLE 7
#define R_LUA_TUSERDATA 8
#define R_LUA_TPROTO 9
#define R_LUA_TUPVALUE 10