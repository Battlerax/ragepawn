#pragma once
#include "main.hpp"

class PlayerNatives 
{
	public:

	static cell AMX_NATIVE_CALL n_GetPlayerName(AMX * amx, const cell *params)
	{
		auto playerId = (int)params[1];
		cell* addr = amx_Address(amx, params[2]);
		amx_SetString(addr, "Austin", 0, 0, (int)params[3]);
		return 0;
	}
};