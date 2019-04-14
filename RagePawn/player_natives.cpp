#include "main.hpp"
#include "api.hpp"
#include "xxHash_xxhash.hpp"

// native GetPlayerName(playerid, name[]);
NATIVE (n_GetPlayerName)
{
	CHECK_PARAMS(2);
	HAS_PLAYER(player, params[1])
	{
		SET_STRING(player->GetName().c_str(), params[2]);
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

		if (pList == NULL)
		{
			len = 0;
			PushEventNull();
		}
		else
		{
			len = strlen(pList);
		}

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
			else
			{
				PushEventNull();
			}
		}
		player->_CallHash(XXHash64::hash(fName, strlen(fName), 0), g_triggerBuff.args, g_triggerBuff.count); // todo args
		g_triggerBuff.count = 0;
		return true;
	}
	return false;
}

NATIVE(n_SendClientMessage) 
{
	CHECK_PARAMS(2);
	HAS_PLAYER(player, params[1])
	{
		char* fName;
		GET_STRING(amx, params[2], fName);
		if(fName) player->OutputChatBox(fName);
		return true;
	}
	return false;
}

const AMX_NATIVE_INFO player_Natives[] =
{
	{ "GetPlayerName", n_GetPlayerName },
	{ "SendClientMessage", n_SendClientMessage },
	{ "TriggerClientEvent", n_TriggerClientEvent },
	{ NULL, NULL }
};

int amx_playerInit(AMX *amx)
{
	return amx_Register(amx, player_Natives, -1);
}