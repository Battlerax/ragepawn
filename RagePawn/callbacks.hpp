#pragma once
#include "pawn.hpp"

class EventHandler :
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

	// forward OnIncomingConnection(playerid, ip_address[])
	// todo: different than on samp: http://wiki.sa-mp.com/wiki/OnIncomingConnection
	void OnPlayerJoin(rage::IPlayer* player) override
	{
		for (auto &script : scripts) 
			Pawn::CallPublicEx(&script.amx, "OnIncomingConnection", "sd", player->GetIp().c_str(), (int)player->GetId());
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
		switch (type)
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
			Pawn::CallPublicEx(&script.amx, "OnPlayerDisconnect", "sdd", reason ? reason : "", result, (int)player->GetId());
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
			Pawn::CallPublicEx(&script.amx, "OnPlayerDeath", "ddd", (int)reason, killer ? killer->GetId() : -1, (int)player->GetId());
	}

	// forward OnVehicleDeath(vehicleid, killerid);
	// forward OnVehicleDeathEx(vehicleid, killerid, reason);
	// todo: OnVehicleDeathEx is new
	void OnVehicleDeath(rage::IVehicle* vehicle, rage::hash_t reason, rage::IPlayer* killer) override
	{
		for (auto &script : scripts)
		{
			Pawn::CallPublicEx(&script.amx, "OnPlayerDeath", "dd", killer ? killer->GetId() : -1, (int)vehicle->GetId());
			Pawn::CallPublicEx(&script.amx, "OnPlayerDeathEx", "ddd", (int)reason, killer ? killer->GetId() : -1, (int)vehicle->GetId());
		}
	}

	// forward OnPlayerText(playerid, text[]);
	void OnPlayerChat(rage::IPlayer* player, const std::u16string& text) override
	{
		for (auto &script : scripts)
			Pawn::CallPublicEx(&script.amx, "OnPlayerText", "sd", text.c_str(), (int)player->GetId());
	}

	// forward OnPlayerCommandText(playerid, cmdtext[]);
	void OnPlayerCommand(rage::IPlayer* player, const std::u16string& command) override
	{
		for (auto &script : scripts)
			Pawn::CallPublicEx(&script.amx, "OnPlayerCommandText", "sd", command.substr(command.find('/') + 1).c_str(), (int)player->GetId());
	}

	// forward OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
	// forward OnPlayerEnterVehicleEx(playerid, vehicleid, seatId);
	// todo: OnPlayerEnterVehicleEx is new
	void OnPlayerEnterVehicle(rage::IPlayer* player, rage::IVehicle* vehicle, uint8_t seatId) override
	{
		for (auto &script : scripts)
		{
			Pawn::CallPublicEx(&script.amx, "OnPlayerEnterVehicle", "ddd", (int)seatId == 0 ? 0 : 1, (int)vehicle->GetId(), (int)player->GetId());
			Pawn::CallPublicEx(&script.amx, "OnPlayerEnterVehicleEx", "ddd", (int)seatId, (int)vehicle->GetId(), (int)player->GetId());
		}
	}

	// forward OnPlayerExitVehicle(playerid, vehicleid);
	void OnPlayerExitVehicle(rage::IPlayer* player, rage::IVehicle* vehicle) override
	{
		for (auto &script : scripts)
			Pawn::CallPublicEx(&script.amx, "OnPlayerExitVehicle", "dd", (int)vehicle->GetId(), (int)player->GetId());
	}

	// forward OnPlayerDamage(playerid, Float:healthLoss, Float:armorLoss);
	// todo: Different than on samp: http://wiki.sa-mp.com/wiki/OnPlayerTakeDamage
	void OnPlayerDamage(rage::IPlayer* player, float healthLoss, float armorLoss) override
	{
		for (auto &script : scripts)
			Pawn::CallPublicEx(&script.amx, "OnPlayerDamage", "ffd", armorLoss, healthLoss, (int)player->GetId());
	}

	//forward OnPlayerStartEnterVehicle(playerid, vehicleid, seatid);
	void OnPlayerStartEnterVehicle(rage::IPlayer* player, rage::IVehicle* vehicle, uint8_t seatId) override
	{
		for (auto &script : scripts)
			Pawn::CallPublicEx(&script.amx, "OnPlayerStartEnterVehicle", "ddd", (int)seatId, (int)vehicle->GetId(), (int)player->GetId());
	}
	//forward OnPlayerStartExitVehicle(playerid,vehicleid);
	void OnPlayerStartExitVehicle(rage::IPlayer* player, rage::IVehicle* vehicle) override
	{
		for (auto &script : scripts)
			Pawn::CallPublicEx(&script.amx, "OnPlayerStartEnterVehicle", "dd", (int)vehicle->GetId(), (int)player->GetId());
	}

	//forward OnPlayerWeaponChange(playerid, oldWeapon, newWeapon);
	void OnPlayerWeaponChange(rage::IPlayer* player, rage::hash_t oldWeapon, rage::hash_t newWeapon) override
	{
		for (auto &script : scripts)
			Pawn::CallPublicEx(&script.amx, "OnPlayerWeaponChange", "ddd", (int)newWeapon, (int)oldWeapon, (int)player->GetId());
	}

	//void OnPlayerRemoteEvent(rage::IPlayer* player, uint64_t eventNameHash, const rage::args_t& args) override;

	void OnEntityCreated(rage::IEntity* entity) override
	{
		for (auto &script : scripts)
			Pawn::CallPublicEx(&script.amx, "OnEntityCreated", "d", (int)entity->GetId());
	}

	void OnEntityDestroyed(rage::IEntity* entity) override
	{
		for (auto &script : scripts)
			Pawn::CallPublicEx(&script.amx, "OnEntityDestroyed", "d", (int)entity->GetId());
	}

	void OnEntityModelChange(rage::IEntity* entity, rage::hash_t oldModel) override
	{
		for (auto &script : scripts)
			Pawn::CallPublicEx(&script.amx, "OnEntityModelChange", "dd", (int)oldModel, (int)entity->GetId());
	}

	void OnPlayerCreateWaypoint(rage::IPlayer* player, const rage::vector3& position) override
	{
		for (auto &script : scripts)
			Pawn::CallPublicEx(&script.amx, "OnPlayerCreateWaypoint", "ffd", position.z, position.y, position.x, (int)player->GetId());
	}

	void OnPlayerReachWaypoint(rage::IPlayer* player) override
	{
		for (auto &script : scripts)
			Pawn::CallPublicEx(&script.amx, "OnPlayerReachWaypoint", "d", (int)player->GetId());
	}


	void OnPlayerEnterColshape(rage::IPlayer* player, rage::IColshape* colshape) override
	{
		for (auto &script : scripts)
			Pawn::CallPublicEx(&script.amx, "OnPlayerEnterColShape", "dd", (int)colshape->GetId(), (int)player->GetId());
	}

	void OnPlayerExitColshape(rage::IPlayer* player, rage::IColshape* colshape) override
	{
		for (auto &script : scripts)
			Pawn::CallPublicEx(&script.amx, "OnPlayerExitColshape", "dd", (int)colshape->GetId(), (int)player->GetId());
	}

	void OnPlayerEnterCheckpoint(rage::IPlayer* player, rage::ICheckpoint* checkpoint) override
	{
		for (auto &script : scripts)
			Pawn::CallPublicEx(&script.amx, "OnPlayerEnterCheckpoint", "dd", (int)checkpoint->GetId(), (int)player->GetId());
	}


	void OnPlayerExitCheckpoint(rage::IPlayer* player, rage::ICheckpoint* checkpoint) override
	{
		for (auto &script : scripts)
			Pawn::CallPublicEx(&script.amx, "OnPlayerExitCheckpoint", "dd", (int)checkpoint->GetId(), (int)player->GetId());
	}

	void OnTrailerAttached(rage::IVehicle* vehicle, rage::IVehicle* trailer) override
	{
		for (auto &script : scripts)
			Pawn::CallPublicEx(&script.amx, "OnTrailerAttached", "dd", (int)trailer->GetId(), (int)vehicle->GetId());
	}

	//todo: Add the ability to pass boolean values to CallPublicEx
	void OnVehicleSirenToggle(rage::IVehicle* vehicle, bool toggle) override
	{
		for (auto &script : scripts)
			Pawn::CallPublicEx(&script.amx, "OnVehicleSirenToggle", "dd", (int)toggle, (int)vehicle->GetId());
	}

	void OnVehicleDamage(rage::IVehicle* vehicle, float bodyHealthLoss, float engineHealthLoss) override
	{
		for (auto &script : scripts)
			Pawn::CallPublicEx(&script.amx, "OnVehicleDamage", "ffd", engineHealthLoss, bodyHealthLoss, (int)vehicle->GetId());
	}

	void OnVehicleHornToggle(rage::IVehicle* vehicle, bool toggle) override
	{
		for (auto &script : scripts)
			Pawn::CallPublicEx(&script.amx, "OnVehicleHornToggle", "dd", (int)toggle, (int)vehicle->GetId());
	}
};
