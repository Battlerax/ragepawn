#include "main.hpp"
#include "api.hpp"

#define MAX_PLAYER_NAME 33

// native GetPlayerName(playerid, const name[])
// todo: different than on samp: native GetPlayerName(playerid, const name[], len)
static cell AMX_NATIVE_CALL n_GetPlayerName(AMX *amx, const cell *params)
{
	HAS_PLAYER(player, (rage::entityId_t)params[1])
	{
		cell* addr = amx_Address(amx, params[2]);
		amx_SetString(addr, player->GetName().c_str(), 0, 0, MAX_PLAYER_NAME);
	}
	return 0;
}

AMX_NATIVE_INFO player_Natives[] =
{
	{ "GetPlayerName", n_GetPlayerName },
	{ NULL, NULL }
};

int amx_playerInit(AMX *amx)
{
	return amx_Register(amx, player_Natives, -1);
}