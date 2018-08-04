#include "main.hpp"
#include "api.hpp"
#include "xxHash_xxhash.hpp"

// native GetPlayerName(playerid, name[], len);
NATIVE (n_GetPlayerName)
{
	HAS_PLAYER(player, params[1])
	{
		SET_STRING(player->GetName().c_str(), params[2], params[3]);
		return true;
	}
	return false;
}

// native TriggerClientEvent(playerid, const name[]);
NATIVE (n_TriggerClientEvent)
{
	HAS_PLAYER(player, params[1])
	{
		GET_STRING(params[2], 256);
		player->_CallHash(XXHash64::hash(output, strlen(output), 0), nullptr, 0); // todo args
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