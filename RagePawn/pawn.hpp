#pragma once
#include "main.hpp"
#include "../amxlib/amx.h" 

class Pawn
{
	public:
		Pawn();
		static Pawn& GetInstance() { static Pawn instance; return instance; }

		void SetMultiplayer(rage::IMultiplayer *mp);
		void Callback(const char * name, const char * fmt, ...);
		rage::IMultiplayer *GetMultiplayer() { return m_mp; }

		int RunAMX(const std::string& path);
		int Terminate() const;
		void TerminateScript();
			
	private:
		rage::IMultiplayer *m_mp;
		AMX amx;
		int err;
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
			Pawn::GetInstance().Callback("OnPlayerConnect", "d", (int)player->GetId());
		}

		virtual void Tick() {  }
	};
}