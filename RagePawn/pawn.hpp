#pragma once
#include "main.hpp"
#include "../amxlib/amx.h" 

static std::vector<AMX*> filterscripts;
static std::vector<AMX*> gamemodes;

class Pawn
{
	public:
		Pawn();
		rage::IMultiplayer *GetMultiplayer() { return m_mp; }
		void SetMultiplayer(rage::IMultiplayer *mp);
		static Pawn& GetInstance() { static Pawn instance; return instance; }

		static bool CallPublic(AMX *amx, const char* name);
		static void CallPublicEx(AMX *amx, const char * name, const char * fmt, ...);
		
		static void RunAMX(const std::string& path, bool fs);
		static void Iterate(const std::string& path, bool fs);
		static int TerminateLoad(const std::string& filename);
		static int Terminate(int err);
		static void TerminateScript(AMX *amx);
			
	private:
		rage::IMultiplayer *m_mp;
};

namespace gm
{
	class EventHandler
		: public rage::IEventHandler,
		public rage::IPlayerHandler,
		public rage::ITickHandler
	{
	public:

		static EventHandler& GetInstance() { static EventHandler instance; return instance; }

		virtual rage::ITickHandler *GetTickHandler() { return this; }
		virtual rage::IPlayerHandler *GetPlayerHandler() { return this; }

		virtual void OnPlayerJoin(rage::IPlayer *player)
		{
			//std::cout << "Player: " << player->GetId() << std::endl;
			for (auto &amx : filterscripts)
			{
				Pawn::CallPublicEx(amx, "OnPlayerConnect", "d", (int)player->GetId());
			}

			for (auto &amx : gamemodes)
			{
				Pawn::CallPublicEx(amx, "OnPlayerConnect", "d", (int)player->GetId());
			}
			
		}

		virtual void Tick() {  }
	};
}