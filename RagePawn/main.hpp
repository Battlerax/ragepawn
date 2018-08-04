#pragma once
#ifndef MAIN_H
#define MAIN_H
#include <Windows.h>
#include <iostream>
#include "../ragemp-cppsdk/rage.hpp"
#endif

#define NATIVE(name) static cell AMX_NATIVE_CALL name(AMX *amx, const cell *params)
// char*: output
#define CHECK_PARAMS(n) { if (params[0] != (n * sizeof(cell))) { printf("Invalid parameter count (count: %d, valid: %d): ", params[0] / sizeof(cell), n); return 0; } }
#define GET_STRING(input, length) char output[length]; cell* address = amx_Address(amx, input); amx_GetString(output, address, 0, sizeof output)
#define SET_STRING(input, destination, length) cell* addr = amx_Address(amx, destination); amx_SetString(addr, input, 0, 0, (int)length)
#define GET_STRLENGTHED(amx,param,result) do { cell *amx_cstr_; int amx_length_; amx_cstr_ = amx_Address((amx), (param)); amx_StrLen(amx_cstr_, &amx_length_); if (amx_length_ > 0 && ((result) = (char*)alloca((amx_length_ + 1) * sizeof(*(result)))) != NULL) amx_GetString((char*)(result), amx_cstr_, sizeof(*(result))>1, amx_length_ + 1); else (result) = NULL; } while (0)