#pragma once

#include "main.hpp"
#include <filesystem>

#include "../amxlib/amx.h"
#include "../amxlib/amxaux.h"

namespace gm
{
	class EventHandler
		: public rage::IEventHandler,
		public rage::IPlayerHandler,
		public rage::ITickHandler
	{
	public:
		virtual rage::ITickHandler *GetTickHandler() { return this; }
		virtual rage::IPlayerHandler *GetPlayerHandler() { return this; }

		virtual void OnPlayerJoin(rage::IPlayer *player) 
		{ 
			std::cout << "Player: " << player->GetId() << std::endl; 
			// call OnPlayerJoin
		}

		virtual void Tick() {  }
	};
}

class Pawn
{
public:
	Pawn();

	void SetMultiplayer(rage::IMultiplayer *mp);
	rage::IMultiplayer *GetMultiplayer() { return m_mp; }

	int RunAMX(const std::string& path);
	int Terminate();
	void TerminateScript();

	static Pawn& GetInstance() { static Pawn instance; return instance; }
	static gm::EventHandler& GetEventInstance() { static gm::EventHandler instance; return instance; }

	//static cell AMX_NATIVE_CALL n_print_int(AMX *amx, const cell *params);
	//void print_int(int value);
		
private:
	rage::IMultiplayer *m_mp;
	AMX amx;
	int err;
};


