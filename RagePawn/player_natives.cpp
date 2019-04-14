#include "main.hpp"
#include "api.hpp"
#include "pawn.hpp"
#include "xxHash_xxhash.hpp"
#include <limits.h>

#define INVALID_VEHICLE_ID = USHRT_MAX;
#define INVALID_PLAYER_ID = USHRT_MAX;
/*
	
	
	virtual void _Call(const std::string& eventName, const arg_t *arguments = nullptr, size_t count = 0) = 0;
	virtual void _CallHash(uint64_t eventNameHash, const arg_t *arguments = nullptr, size_t count = 0) = 0; // xxhash64
	virtual void _Invoke(uint64_t nativeHash, const arg_t *arguments = nullptr, size_t count = 0) = 0;
	virtual void Spawn(const vector3& pos, float heading) = 0;
	virtual void PlayAnimation(const std::string& dict, const std::string& name, float speed = 8.f, int flags = 0) = 0;
	virtual void PlayScenario(const std::string& scenario) = 0;
	virtual void StopAnimation() = 0;
	virtual const clothData_t& GetClothes(uint8_t id) = 0;
	virtual void SetClothes(uint8_t id, const clothData_t& clothes) = 0;
	virtual void SetClothes(const std::vector<std::pair<uint8_t, const clothData_t>>& clothes) = 0;
	virtual const propData_t& GetProp(uint8_t id) = 0;
	virtual void SetProp(uint8_t id, const propData_t prop) = 0;
	virtual void SetProp(const std::vector<std::pair<uint8_t, const propData_t>>& clothes) = 0;
	
	virtual void SetCustomization(bool gender,
		const rage::headBlend_t& headBlend,
		uint8_t eyeColor, uint8_t hairColor, uint8_t hightlightColor,
		const std::vector<float>& faceFeatures,
		const std::map<int, headOverlay_t>& headOverlays,
		const std::vector<std::pair<uint32_t, uint32_t>> decorations) = 0;

	virtual uint32_t GetDecoration(uint32_t collection) = 0;
	virtual void RemoveDecoration(uint32_t collection, uint32_t overlay) = 0;
	virtual void SetDecoration(uint32_t collection, uint32_t overlay) = 0;
	virtual void SetDecorations(std::vector<std::pair<uint32_t, uint32_t>> decorations) = 0;


*/

//native Kick(playerid, reason[]='\0');
NATIVE (n_kickPlayer)
{
	HAS_PLAYER(player, params[1])
	{
		char* reason;
		GET_STRING(amx, params[2], reason);
		//these checks may be over kill not sure.
		player->Kick((reason == NULL || reason ==  nullptr || !reason[0]) ? NULL : reason);
		return true;
	}
	return false;
}


//virtual void Ban(const char *reason = nullptr) = 0;
//native Ban(playerid, reason[]='\0');
NATIVE(n_banPlayer)
{
	HAS_PLAYER(player, params[1])
	{
		char* reason;
		GET_STRING(amx, params[2], reason);
		//these checks may be over kill not sure.
		player->Ban((reason == NULL || reason == nullptr || !reason[0]) ? NULL : reason);
		return true;
	}
	return false;
}

//virtual void OutputChatBox(const std::string& text) = 0;
//native SendClientMessage(playerid, message[]);
NATIVE(n_sendClientMessage)
{
	CHECK_PARAMS(2);
	HAS_PLAYER(player, params[1])
	{
		char* message;
		GET_STRING(amx, params[2], message);
		
		if (message == NULL)
			return false;
		
		player->OutputChatBox(message);

	}
	return false;
}


//virtual void Notify(const std::string& text) = 0;
//native Notify(playerid, message[]);
NATIVE(n_notifyPlayer)
{
	CHECK_PARAMS(2);
	HAS_PLAYER(player, params[1])
	{
		char* message;
		GET_STRING(amx, params[2], message);

		if (message == NULL)
			return false;

		player->Notify(message);

	}
	return false;
}

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
NATIVE (n_triggerClientEvent)
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
//virtual void SetName(const std::string& name) = 0;
//native SetPlayerName(playerid, name[]);
NATIVE (n_setPlayerName)
{
	CHECK_PARAMS(2);
	HAS_PLAYER(player, params[1])
	{
		char* name;
		GET_STRING(amx, params[2], name);

		if (name == NULL)
			return false;

		player->SetName(name);
		return true;
	}
	return false;
}

//virtual bool IsAiming() = 0;
//native IsPlayerAiming(playerid);
NATIVE(n_isPlayerAiming)
{
	CHECK_PARAMS(2);
	HAS_PLAYER(player, params[1])
	{
		return player->IsAiming();
	}
	return false;
}

//virtual float GetHeading() = 0;
//native GetPlayerHeading(playerid, &Float:heading);
NATIVE(n_getPlayerHeading)
{
	CHECK_PARAMS(2);
	HAS_PLAYER(player, params[1])
	{
		float heading = player->GetHeading();
		cell* c_ptr = amx_Address(amx, params[2]);
		*c_ptr = amx_ftoc(heading);
		return true;
		
	}
	return false;
}

//virtual void SetHeading(float heading) = 0;
//native SetPlayerHeading(playerid, Float:heading);
NATIVE(n_setPlayerHeading)
{
	CHECK_PARAMS(2);
	HAS_PLAYER(player, params[1])
	{
		float heading = amx_ctof(params[2]);
		player->SetHeading(heading);
		return true;
	}
	return false;
}


//virtual float GetMoveSpeed() = 0;
//native GetPlayerMoveSpeed(playerid, &Float:moveSpeed);
NATIVE(n_getPlayerMoveSpeed)
{
	CHECK_PARAMS(2);
	HAS_PLAYER(player, params[1])
	{
		float moveSpeed = player->GetMoveSpeed();
		cell* c_ptr = amx_Address(amx, params[2]);
		*c_ptr = amx_ftoc(moveSpeed);
		return true;
	}
	return false;
}

//virtual float GetHealth() = 0;
//native GetPlayerHealth(playerid, &Float:health);
NATIVE(n_getPlayerHealth)
{
	CHECK_PARAMS(2);
	HAS_PLAYER(player, params[1])
	{
		float health = player->GetHealth();
		cell* c_ptr = amx_Address(amx, params[2]);
		*c_ptr = amx_ftoc(health);
		return true;
	}
	return false;
}

//virtual void SetHealth(float armour) = 0;
//native SetPlayerHealth(playerid, Float:value);
NATIVE(n_setPlayerHealth)
{
	CHECK_PARAMS(2);
	HAS_PLAYER(player, params[1])
	{
		float health = amx_ctof(params[2]);
		player->SetHealth(health);
		return true;
	}
	return false;
}


//virtual float GetArmour() = 0;
//native GetPlayerArmour(playerid, &Float:armour);
NATIVE(n_getPlayerArmour)
{
	CHECK_PARAMS(2);
	HAS_PLAYER(player, params[1])
	{
		float armour = player->GetArmour();
		cell* c_ptr = amx_Address(amx, params[2]);
		*c_ptr = amx_ftoc(armour);
		return true;
	}
	return false;
}


//virtual void SetArmour(float armour) = 0;
//native SetPlayerArmour(playerid, Float:armour);
NATIVE(n_setPlayerArmour)
{
	CHECK_PARAMS(2);
	HAS_PLAYER(player, params[1])
	{
		float armour = amx_ctof(params[2]);
		player->SetArmour(armour);
		return true;
	}
	return false;
}

//virtual const vector3& GetAimingAt() = 0;
//native GetPlayerAimPosition(playerid, &Float:x, &Float:y, &Float:z);
NATIVE(n_getPlayerAimPosition)
{
	CHECK_PARAMS(4);
	HAS_PLAYER(player, params[1])
	{
		rage::vector3 pos = player->GetAimingAt();

		cell* xptr = amx_Address(amx, params[2]);
		*xptr = amx_ftoc(pos.x);

		cell* yptr = amx_Address(amx, params[3]);
		*xptr = amx_ftoc(pos.y);

		cell* zptr = amx_Address(amx, params[4]);
		*zptr = amx_ftoc(pos.z);
		return true;
	}
	return false;
}




//virtual int GetPing() = 0;
//native GetPlayerPing(playerid, &ping);
NATIVE(n_getPlayerPing)
{
	CHECK_PARAMS(2);
	HAS_PLAYER(player, params[1])
	{
		int ping = player->GetPing();
		cell* c_ptr = amx_Address(amx, params[2]);
		*c_ptr = ping;
		return true;
	}
	return false;
}

//virtual float GetPacketLoss() = 0;
//native GetPlayerPacketLoss(playerid, &packetLoss);
NATIVE(n_getPlayerPacketLoss)
{
	CHECK_PARAMS(2);
	HAS_PLAYER(player, params[1])
	{
		float packetLoss = player->GetPacketLoss();
		cell* c_ptr = amx_Address(amx, params[2]);
		*c_ptr = amx_ftoc(packetLoss);
		return true;
	}
	return false;
}

//virtual const std::string& GetKickReason() = 0;
//native GetPlayerKickReason(playerid, &reason[], length)
NATIVE(n_getPlayerKickReason)
{
	CHECK_PARAMS(3);
	HAS_PLAYER(player, params[1])
	{
		SET_STRING(player->GetKickReason().c_str(), params[2], params[3]);
		return true;
	}
	return false;
}


//virtual void Eval(const std::string& code) = 0;
//native Evaluate(playerid, code[]);
NATIVE(n_evaluateJavascript)
{
	CHECK_PARAMS(2);
	HAS_PLAYER(player, params[1])
	{
		char* js;
		GET_STRING(amx, params[1], js);
		player->Eval(js);
		return true;
	}
	return false;
}


//virtual std::string GetIp() = 0;
//native GetPlayerIP(playerid, &ipAddress[], length);
NATIVE(n_getPlayerIP)
{
	CHECK_PARAMS(3);
	HAS_PLAYER(player, params[1])
	{
		SET_STRING(player->GetIp().c_str(), params[2], params[3]);
		return true;
	}
	return false;
}

//virtual bool IsJumping() = 0;
//native IsPlayerJumping(playerid);
NATIVE(n_isPlayerJumping)
{
	CHECK_PARAMS(1);
	HAS_PLAYER(player, params[1])
	{
		return player->IsJumping();
	}
	return false;
}

//virtual bool IsInCover() = 0;
//native IsPlayerInCover(playerid);
NATIVE(n_isPlayerInCover)
{
	CHECK_PARAMS(1);
	HAS_PLAYER(player, params[1])
	{
		return player->IsInCover();
	}
	return false;
}


//virtual bool IsEnteringVehicle() = 0;
//native IsPlayerEnteringVehicle(playerid);
NATIVE(n_isPlayerEnteringVehicle)
{
	CHECK_PARAMS(1);
	HAS_PLAYER(player, params[1])
	{
		return player->IsEnteringVehicle();
	}
	return false;
}

//virtual bool IsLeavingVehicle() = 0;
//native IsPlayerLeavingVehicle(playerid);
NATIVE(n_isPlayerLeavingVehicle)
{
	CHECK_PARAMS(1);
	HAS_PLAYER(player, params[1])
	{
		return player->IsLeavingVehicle();
	}
	return false;
}

//virtual bool IsClimbing() = 0;
//native IsPlayerClimbing(playerid);
NATIVE(n_isPlayerClimbing)
{
	CHECK_PARAMS(1);
	HAS_PLAYER(player, params[1])
	{
		return player->IsClimbing();
	}
	return false;
}

//virtual bool IsOnLadder() = 0;
//native IsPlayerOnLadder(playerid);
NATIVE(n_isPlayerOnLadder)
{
	CHECK_PARAMS(1);
	HAS_PLAYER(player, params[1])
	{
		return player->IsOnLadder();
	}
	return false;
}

//virtual bool IsReloading() = 0;
//native IsPlayerReloading(playerid);
NATIVE(n_isPlayerReloading)
{
	CHECK_PARAMS(1);
	HAS_PLAYER(player, params[1])
	{
		return player->IsReloading();
	}
	return false;
}

//virtual bool IsInMelee() = 0;
//native IsPlayerInMelee(playerid);
NATIVE(n_isPlayerInMelee)
{
	CHECK_PARAMS(1);
	HAS_PLAYER(player, params[1])
	{
		return player->IsInMelee();
	}
	return false;
}

//virtual std::string GetActionString() = 0;
//native GetPlayerActionString(playerid, action[], length);
NATIVE(n_getPlayerActionString)
{
	CHECK_PARAMS(3);
	HAS_PLAYER(player, params[1])
	{
		SET_STRING(player->GetActionString().c_str(), params[2], params[1]);
		return true;
	}
	return false;
}


// Vehicle
//virtual IVehicle *GetVehicle() = 0;
//native GetPlayerVehicleID(playerid);
NATIVE(n_getPlayerVehicleID)
{
	CHECK_PARAMS(1);
	HAS_PLAYER(player, params[1])
	{
		return player->GetVehicle()->GetId();
	}
	return false;
}


//virtual void PutIntoVehicle(IVehicle *vehicle, int8_t seatId) = 0;
//native PutPlayerInVehicle(playerid, vehicleid, seatid);
NATIVE(n_putPlayerInVehicle)
{
	CHECK_PARAMS(3);
	HAS_PLAYER(player, params[1])
	{
		HAS_VEHICLE(vehicle, params[2])
		{
			player->PutIntoVehicle(vehicle, (int8_t)params[3]);
			return true;
		}
		return false;
	}
	return false;
}
//virtual void RemoveFromVehicle() = 0;
//virtual int8_t GetSeat() = 0;

// Customization
//virtual uint8_t GetEyeColour() = 0;
//virtual void SetEyeColour(uint8_t colour) = 0;

//virtual uint8_t GetHairColour() = 0;
//virtual uint8_t GetHairHighlightColour() = 0;
//virtual void SetHairColour(uint8_t colour, uint8_t highlightColour) = 0;

//virtual float GetFaceFeature(uint8_t id) = 0;
//virtual void SetFaceFeature(uint8_t id, float scale) = 0;

//virtual const headBlend_t& GetHeadBlend() = 0;

//virtual void SetHeadBlend(int shapeFirstID, int shapeSecondID, int shapeThirdID, int skinFirstID, int skinSecondID, int skinThirdID,float shapeMix, float skinMix, float thirdMix) = 0;

//virtual void UpdateHeadBlend(float shapeMix, float skinMix, float thirdMix) = 0;

//virtual headOverlay_t GetHeadOverlay(uint8_t overlayId) = 0;
//virtual void SetHeadOverlay(uint8_t overlayId, headOverlay_t overlay) = 0;

// Weapons
//virtual uint32_t GetCurrentWeapon() = 0;
//virtual void SetCurrentWeapon(uint32_t weapon) = 0;

//virtual uint16_t GetCurrentWeaponAmmo() = 0;
//virtual void SetCurrentWeaponAmmo(uint16_t ammo) = 0;

//virtual uint16_t GetWeaponAmmo(uint32_t weaponHash) = 0;
//virtual void SetWeaponAmmo(hash_t hash, uint16_t ammo) = 0;

//virtual std::map<hash_t, uint16_t> GetWeapons() = 0;

//virtual void GiveWeapon(hash_t hash, uint16_t ammo) = 0;
//virtual void GiveWeapons(std::vector<std::pair<hash_t, uint16_t>> weapons) = 0;

//virtual void RemoveWeapon(hash_t hash) = 0;
//virtual void RemoveWeapons(const std::vector<hash_t>& hash) = 0;
//virtual void RemoveAllWeapons() = 0;

//
//virtual bool IsStreamed(IPlayer *player) = 0;
//virtual std::vector<IPlayer*> GetStreamed() = 0;

//virtual const std::string& GetSerial() = 0;
//virtual const std::string& GetSocialClubName() = 0;

//virtual void RemoveObject(uint32_t model, const vector3& position, float radius) = 0;

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
	{ "Kick", n_kickPlayer },
	{ "Ban", n_banPlayer },
	{ "SendClientMessage", n_sendClientMessage },
	{ "NotifyPlayer", n_notifyPlayer},
	{ "GetPlayerName", n_getPlayerName },
	{ "TriggerClientEvent", n_triggerClientEvent },
	{ "IsPlayerAiming", n_isPlayerAiming },
	{ "GetPlayerHeading",n_getPlayerHeading },
	{ "SetPlayerHeading", n_setPlayerHeading },
	{ "GetPlayerMoveSpeed", n_getPlayerMoveSpeed },
	{ "GetPlayerHealth", n_getPlayerHealth },
	{ "SetPlayerHealth", n_setPlayerHealth },
	{ "GetPlayerArmour", n_getPlayerArmour },
	{ "SetPlayerArmour", n_setPlayerArmour },
	{ "GetPlayerAimPosition", n_getPlayerAimPosition },
	{ "GetPlayerPing", n_getPlayerPing },
	{ "GetPlayerPacketLoss", n_getPlayerPacketLoss },
	{ "GetPlayerKickReason", n_getPlayerKickReason },
	{ "Evaluate" , n_evaluateJavascript },
	{ "SetPlayerName", n_setPlayerName },
	{ "GetPlayerIP", n_getPlayerIP },
	{ "IsPlayerJumping", n_isPlayerJumping },
	{ "IsPlayerInCover", n_isPlayerInCover },
	{ "IsPlayerEnteringVehicle", n_isPlayerEnteringVehicle },
	{ "IsPlayerLeavingVehicle", n_isPlayerLeavingVehicle },
	{ "IsPlayerClimbing", n_isPlayerClimbing },
	{ "IsPlayerOnLadder", n_isPlayerOnLadder },
	{ "IsPlayerReloading", n_isPlayerReloading },
	{ "IsPlayerInMelee", n_isPlayerInMelee },
	{ "GetPlayerActionString", n_getPlayerActionString },
	{ NULL, NULL }
};

int amx_playerInit(AMX *amx)
{
	return amx_Register(amx, player_Natives, -1);
}