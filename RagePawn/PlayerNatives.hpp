#pragma once

#include "main.hpp"

class PlayerNatives 
{
private:
	static char *MakePackedString(cell *cptr)
	{
		int len;
		char *dest;

		amx_StrLen(cptr, &len);
		dest = (char *)malloc(len + sizeof(cell));
		amx_GetString(dest, cptr, 0, len + sizeof(cell));
		return dest;
	}
	void GetPlayerName(AMX *amx, int playerid)
	{
		//code...
	}
public:
	static cell AMX_NATIVE_CALL n_print_str(AMX *amx, const cell *params)
	{
		cell* cstr = amx_Address(amx, params[1]);
		char *str = MakePackedString(cstr);
		std::cout << "PSTR:: " << str << std::endl;
		free(str);
		return 0;
	}
	static cell AMX_NATIVE_CALL n_GetPlayerName(AMX * amx, const cell *params)
	{
		int playerid = (int)params[1];
		cell* addr = amx_Address(amx, params[2]);
		amx_SetString(addr, "Austin", 0, 0, (int)params[3]);
		return 0;
	}
	static cell AMX_NATIVE_CALL n_fprintf(AMX *amx, const cell *params) 
	{
		int length = (int)(params[0] / sizeof(cell)) - 1;
	}
};