#pragma once

#include "main.hpp"
#include "natives.hpp"
#include <filesystem>

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

	int InitializeAMX(const std::string& path);

	void SetMultiplayer(rage::IMultiplayer *mp);
	rage::IMultiplayer *GetMultiplayer() { return m_mp; }

	int CallMain();
	void ScriptError();
	void KillScript();
	bool LoadAMX(char *file, void *program);
	bool RegisterNatives();

	static Pawn& GetInstance() { static Pawn instance; return instance; }
	static gm::EventHandler& GetEventInstance() { static gm::EventHandler instance; return instance; }
		
private:
	rage::IMultiplayer *m_mp;
	void* script;
	AMX amx;
	int err;
};


