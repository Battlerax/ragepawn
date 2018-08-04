#include "main.hpp"
#include "api.hpp"
#include "xxHash_xxhash.hpp"

// native GetPlayerName(playerid, name[], len);
static cell AMX_NATIVE_CALL n_GetPlayerName(AMX *amx, const cell *params)
{
	HAS_PLAYER(player, (rage::entityId_t)params[1])
	{
		cell* addr = amx_Address(amx, params[2]);
		amx_SetString(addr, player->GetName().c_str(), 0, 0, (int)params[3]);
		return true;
	}
	return false;
}

// native TriggerClientEvent(playerid, const name[]);
static cell AMX_NATIVE_CALL n_TriggerClientEvent(AMX *amx, const cell *params)
{
	HAS_PLAYER(player, (rage::entityId_t)params[1])
	{
		char str[128];
		cell* cstr = amx_Address(amx, params[2]);
		amx_GetString(str, cstr, 0, sizeof str);
		const uint64_t name = XXHash64::hash(str, strlen(str), 0);
		player->_CallHash(name, nullptr, 0); // todo args
		return true;
	}
	return false;
}

const AMX_NATIVE_INFO player_Natives[] =
{
	{ "GetPlayerName", n_GetPlayerName },
	{ "TriggerClientEvent", n_TriggerClientEvent },
	{ NULL, NULL }
};

int amx_playerInit(AMX *amx)
{
	return amx_Register(amx, player_Natives, -1);
}