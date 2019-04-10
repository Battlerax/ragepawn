#include "main.hpp"
#include "api.hpp"
#include "pawn.hpp"
#include "xxHash_xxhash.hpp"



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

	virtual void Eval(const std::string& code) = 0;


	virtual void SetName(const std::string& name) = 0;

	virtual bool IsAiming() = 0;

	virtual float GetHeading() = 0;
	virtual void SetHeading(float heading) = 0;

	virtual float GetMoveSpeed() = 0;

	virtual float GetHealth() = 0;
	virtual void SetHealth(float armour) = 0;

	virtual float GetArmour() = 0;
	virtual void SetArmour(float armour) = 0;

	virtual const vector3& GetAimingAt() = 0;

	virtual int GetPing() = 0;
	virtual float GetPacketLoss() = 0;

	virtual const std::string& GetKickReason() = 0;

	virtual std::string GetIp() = 0;

	virtual bool IsJumping() = 0;
	virtual bool IsInCover() = 0;
	virtual bool IsEnteringVehicle() = 0;
	virtual bool IsLeavingVehicle() = 0;
	virtual bool IsClimbing() = 0;
	virtual bool IsOnLadder() = 0;
	virtual bool IsReloading() = 0;
	virtual bool IsInMelee() = 0;
	virtual std::string GetActionString() = 0;

	// Vehicle
	virtual IVehicle *GetVehicle() = 0;
	virtual void PutIntoVehicle(IVehicle *vehicle, int8_t seatId) = 0;
	virtual void RemoveFromVehicle() = 0;
	virtual int8_t GetSeat() = 0;

	// Customization
	virtual uint8_t GetEyeColour() = 0;
	virtual void SetEyeColour(uint8_t colour) = 0;

	virtual uint8_t GetHairColour() = 0;
	virtual uint8_t GetHairHighlightColour() = 0;
	virtual void SetHairColour(uint8_t colour, uint8_t highlightColour) = 0;

	virtual float GetFaceFeature(uint8_t id) = 0;
	virtual void SetFaceFeature(uint8_t id, float scale) = 0;

	virtual const headBlend_t& GetHeadBlend() = 0;

	virtual void SetHeadBlend(int shapeFirstID, int shapeSecondID, int shapeThirdID, int skinFirstID, int skinSecondID, int skinThirdID,
		float shapeMix, float skinMix, float thirdMix) = 0;

	virtual void UpdateHeadBlend(float shapeMix, float skinMix, float thirdMix) = 0;

	virtual headOverlay_t GetHeadOverlay(uint8_t overlayId) = 0;
	virtual void SetHeadOverlay(uint8_t overlayId, headOverlay_t overlay) = 0;

	// Weapons
	virtual uint32_t GetCurrentWeapon() = 0;
	virtual void SetCurrentWeapon(uint32_t weapon) = 0;

	virtual uint16_t GetCurrentWeaponAmmo() = 0;
	virtual void SetCurrentWeaponAmmo(uint16_t ammo) = 0;

	virtual uint16_t GetWeaponAmmo(uint32_t weaponHash) = 0;
	virtual void SetWeaponAmmo(hash_t hash, uint16_t ammo) = 0;

	virtual std::map<hash_t, uint16_t> GetWeapons() = 0;

	virtual void GiveWeapon(hash_t hash, uint16_t ammo) = 0;
	virtual void GiveWeapons(std::vector<std::pair<hash_t, uint16_t>> weapons) = 0;

	virtual void RemoveWeapon(hash_t hash) = 0;
	virtual void RemoveWeapons(const std::vector<hash_t>& hash) = 0;
	virtual void RemoveAllWeapons() = 0;

	//
	virtual bool IsStreamed(IPlayer *player) = 0;
	virtual std::vector<IPlayer*> GetStreamed() = 0;

	virtual const std::string& GetSerial() = 0;
	virtual const std::string& GetSocialClubName() = 0;

	virtual void RemoveObject(uint32_t model, const vector3& position, float radius) = 0;*/

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

// native GetPlayerName(playerid, name[], len);
NATIVE (n_getPlayerName)
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
//native Float:GetPlayerHeading(playerid);
NATIVE(n_getPlayerHeading)
{
	CHECK_PARAMS(2);
	HAS_PLAYER(player, params[1])
	{
		float heading = player->GetHeading();
		return amx_ftoc(heading);
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
	{ "GetPlayerHeading",n_getPlayerHeading},
	{ NULL, NULL }
};

int amx_playerInit(AMX *amx)
{
	return amx_Register(amx, player_Natives, -1);
}