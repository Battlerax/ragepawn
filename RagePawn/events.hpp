#pragma once
#include "pawn.hpp"

class EventHandler : public rage::IEventHandler, public rage::IPlayerHandler, public rage::ITickHandler
{
public:
	static EventHandler& GetInstance() { static EventHandler instance; return instance; }

	virtual rage::ITickHandler *GetTickHandler() override { return this; }
	virtual rage::IPlayerHandler *GetPlayerHandler() override { return this; }

	void OnPlayerJoin(rage::IPlayer *player) override
	{
		for (auto &script : scripts) Pawn::CallPublicEx(&script.amx, "OnPlayerConnect", "d", (int)player->GetId());
	}

	void Tick() override
	{
		for (auto &script : scripts) Pawn::CallPublic(&script.amx, "OnUpdate");
	}
};
