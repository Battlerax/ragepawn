#pragma once
#include "pawn.hpp"

class EventHandler: 
	public rage::IEventHandler,
	public rage::IEntityHandler,
	public rage::IPlayerHandler,
	public rage::IVehicleHandler,
	public rage::IColshapeHandler,
	public rage::ICheckpointHandler,
	public rage::IBlipHandler,
	public rage::ITickHandler
{
public:
	static EventHandler& GetInstance() { static EventHandler instance; return instance; }
	IEntityHandler *GetEntityHandler() override { return this; }
	IPlayerHandler *GetPlayerHandler() override { return this; }
	IVehicleHandler *GetVehicleHandler() override { return this; }
	IColshapeHandler *GetColshapeHandler() override { return this; }
	ICheckpointHandler *GetCheckpointHandler() override { return this; }
	IBlipHandler *GetBlipHandler() override { return this; }
	ITickHandler *GetTickHandler() override { return this; }

	// forward OnIncomingConnection(playerid, ip_address[], serial[])
	// todo: different than on samp: http://wiki.sa-mp.com/wiki/OnIncomingConnection
	void OnPlayerJoin(rage::IPlayer* player) override
	{
		for (auto &script : scripts)
			Pawn::CallPublicEx(&script.amx, "OnIncomingConnection", "d", (int)player->GetId(), "s", player->GetIp().c_str(), "s", player->GetSerial().c_str());
	}

	// forward OnPlayerConnect(playerid);
	void OnPlayerReady(rage::IPlayer *player) override
	{
		for (auto &script : scripts)
			Pawn::CallPublicEx(&script.amx, "OnPlayerConnect", "d", (int)player->GetId());
	}

	// forward OnUpdate();
	void Tick() override
	{
		for (auto &script : scripts) 
			Pawn::CallPublic(&script.amx, "OnUpdate");
	}

	// forward OnPlayerDisconnect(playerid, reason, message[]);
	// todo: different than on samp: http://wiki.sa-mp.com/wiki/OnPlayerDisconnect
	void OnPlayerQuit(rage::IPlayer *player, rage::exit_t exitType, const char *reason) override
	{
		int result;
		const int type = (int)exitType;
		switch(type)
		{
			case 0: // disconnect
				result = 1;
				break;
			case 1: // timeout
				result = 0;
				break;
			default:
				result = type;
		}

		for (auto &script : scripts) 
			Pawn::CallPublicEx(&script.amx, "OnPlayerConnect", "d", (int)player->GetId(), "d", result, "s", reason ? reason : "");
	}

	// forward OnPlayerSpawn(playerid);
	void OnPlayerSpawn(rage::IPlayer* player) override
	{
		for (auto &script : scripts)
			Pawn::CallPublicEx(&script.amx, "OnPlayerSpawn", "d", (int)player->GetId());
	}

	// forward OnPlayerDeath(playerid, killerid, reason);
	void OnPlayerDeath(rage::IPlayer* player, rage::hash_t reason, rage::IPlayer* killer) override
	{
		for (auto &script : scripts)
			Pawn::CallPublicEx(&script.amx, "OnPlayerDeath", "d", (int)player->GetId(), "d", killer ? killer->GetId() : -1, "d", (int)reason);
	}

	// forward OnVehicleDeath(vehicleid, killerid);
	// forward OnVehicleDeathEx(vehicleid, killerid, reason);
	// todo: OnVehicleDeathEx is new
	void OnVehicleDeath(rage::IVehicle* vehicle, rage::hash_t reason, rage::IPlayer* killer) override
	{
		for (auto &script : scripts)
		{
			Pawn::CallPublicEx(&script.amx, "OnPlayerDeath", "d", (int)vehicle->GetId(), "d", killer ? killer->GetId() : -1);
			Pawn::CallPublicEx(&script.amx, "OnPlayerDeathEx", "d", (int)vehicle->GetId(), "d", killer ? killer->GetId() : -1, "d", (int)reason);
		}
	}

	// forward OnPlayerText(playerid, text[]);
	void OnPlayerChat(rage::IPlayer* player, const std::u16string& text) override
	{
		for (auto &script : scripts)
			Pawn::CallPublicEx(&script.amx, "OnPlayerText", "d", (int)player->GetId(), "s", text.c_str());
	}

	// forward OnPlayerCommandText(playerid, cmdtext[]);
	// todo: cmdtest[] does not contain the '/' command prefix
	void OnPlayerCommand(rage::IPlayer* player, const std::u16string& command) override
	{
		for (auto &script : scripts)
			Pawn::CallPublicEx(&script.amx, "OnPlayerCommandText", "d", (int)player->GetId(), "s", command.substr(command.find('/') + 1).c_str());
	}

	// forward OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
	// forward OnPlayerEnterVehicleEx(playerid, vehicleid, seatId);
	// todo: OnPlayerEnterVehicleEx is new
	void OnPlayerEnterVehicle(rage::IPlayer* player, rage::IVehicle* vehicle, uint8_t seatId) override
	{
		for (auto &script : scripts)
		{
			Pawn::CallPublicEx(&script.amx, "OnPlayerEnterVehicle", "d", (int)player->GetId(), "d", (int)vehicle->GetId(), "d", (int)seatId == 0 ? 0 : 1);
			Pawn::CallPublicEx(&script.amx, "OnPlayerEnterVehicleEx", "d", (int)player->GetId(), "d", (int)vehicle->GetId(), "d", (int)seatId);
		}
	}

	// forward OnPlayerExitVehicle(playerid, vehicleid);
	void OnPlayerExitVehicle(rage::IPlayer* player, rage::IVehicle* vehicle) override
	{
		for (auto &script : scripts)
			Pawn::CallPublicEx(&script.amx, "OnPlayerExitVehicle", "d", (int)player->GetId(), "d", (int)vehicle->GetId());
	}

	// forward OnPlayerDamage(playerid, Float:healthLoss, Float:armorLoss)
	// todo: Different than on samp: http://wiki.sa-mp.com/wiki/OnPlayerTakeDamage
	void OnPlayerDamage(rage::IPlayer* player, float healthLoss, float armorLoss) override
	{
		for (auto &script : scripts)
			Pawn::CallPublicEx(&script.amx, "OnPlayerDamage", "d", (int)player->GetId(), "f", healthLoss, "f", armorLoss);
	}

	//void OnPlayerStartEnterVehicle(rage::IPlayer* player, rage::IVehicle* vehicle, uint8_t seatId) override;
	//void OnPlayerStartExitVehicle(rage::IPlayer* player, rage::IVehicle* vehicle) override;
	//void OnPlayerWeaponChange(rage::IPlayer* player, rage::hash_t oldWeapon, rage::hash_t newWeapon) override;
	//void OnPlayerRemoteEvent(rage::IPlayer* player, uint64_t eventNameHash, const rage::args_t& args) override;
	//void OnEntityCreated(rage::IEntity* entity) override;
	//void OnEntityDestroyed(rage::IEntity* entity) override;
	//void OnEntityModelChange(rage::IEntity* entity, rage::hash_t oldModel) override;
	//void OnPlayerCreateWaypoint(rage::IPlayer* player, const rage::vector3& position) override;
	//void OnPlayerReachWaypoint(rage::IPlayer* player) override;
	//void OnPlayerEnterColshape(rage::IPlayer* player, rage::IColshape* colshape) override;
	//void OnPlayerExitColshape(rage::IPlayer* player, rage::IColshape* colshape) override;
	//void OnPlayerEnterCheckpoint(rage::IPlayer* player, rage::ICheckpoint* checkpoint) override;
	//void OnPlayerExitCheckpoint(rage::IPlayer* player, rage::ICheckpoint* checkpoint) override;
	//void OnTrailerAttached(rage::IVehicle* vehicle, rage::IVehicle* trailer) override;
	//void OnVehicleSirenToggle(rage::IVehicle* vehicle, bool toggle) override;
	//void OnVehicleDamage(rage::IVehicle* vehicle, float bodyHealthLoss, float engineHealthLoss) override;
	//void OnVehicleHornToggle(rage::IVehicle* vehicle, bool toggle) override;

};
