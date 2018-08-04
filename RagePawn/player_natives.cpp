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

// native TriggerClientEvent(playerid, const name[], const format[], ...);
// example: TriggerClientEvent(playerid, "TestEvent", "sds", "wow", 12, "fart");
NATIVE (n_TriggerClientEvent)
{
	HAS_PLAYER(player, params[1])
	{
		int len;
		char* fName;
		char* pList;
		GET_STRING(amx, params[2], fName);
		GET_STRING(amx, params[3], pList);

		const int offset = 3;

		if (pList == NULL) len = 0;
		else len = strlen(pList);

		while (len)
		{
			len--;
			if (*(pList + len) == 's')
			{
				char* pText;
				GET_STRING(amx, params[len + offset], pText);
				if (pText == NULL || strlen(pText) <= 0)
				{
					*pText = 1;
					*(pText + 1) = 0;
				}
				const char* str = pText;
				PushEventString(str, (int)strlen(str));
				std::cout << str << std::endl;
			}
			else if (*(pList + len) == 'd')
			{
				cell* cptr = amx_Address(amx, params[4]);
				PushEventInt(*cptr);
			}
			else if (*(pList + len) == 'f')
			{
				cell* cptr = amx_Address(amx, params[4]);
				PushEventFloat(*cptr);
			}
		}
		player->_CallHash(XXHash64::hash(fName, strlen(fName), 0), g_triggerBuff.args, g_triggerBuff.count); // todo args
		g_triggerBuff.count = 0;
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