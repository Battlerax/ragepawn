#pragma once
#ifndef MAIN_H
#define MAIN_H
#include <Windows.h>
#include <iostream>
#include "../ragemp-cppsdk/rage.hpp"
#endif

#define NATIVE(name) static cell AMX_NATIVE_CALL name(AMX *amx, const cell *params)

// char*: output
#define GET_STRING(input, length) char output[length]; cell* address = amx_Address(amx, input); amx_GetString(output, address, 0, sizeof output);
#define SET_STRING(input, destination, length) cell* addr = amx_Address(amx, destination); amx_SetString(addr, input, 0, 0, (int)length);