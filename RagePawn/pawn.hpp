#pragma once
#include "main.hpp"
#include "../amxlib/amx.h" 
typedef struct {
	bool fs;
	AMX amx;
} script;

static std::vector<script> scripts;

class Pawn : public rage::IPlugin
{
	public:
		Pawn();
		rage::IMultiplayer *GetMultiplayer() const { return m_mp; }
		void SetMultiplayer(rage::IMultiplayer *mp);
		static Pawn& GetInstance() { static Pawn instance; return instance; }

		static void CallPublic(AMX *amx, const char* name);
		static void CallPublicEx(AMX *amx, const char * name, const char * fmt, ...);
		
		static void RunAMX(const std::string& path, bool fs);
		static void Iterate(const std::string& path, bool fs);

		static int TerminateLoad(const std::string& filename);
		static int Terminate(int err);
		static void TerminateScript(AMX *amx);

		rage::IEntity *GetEntity(rage::entityId_t id, rage::entity_t type) const;
		static int joaat(std::string string);
			
	private:
		rage::IMultiplayer *m_mp;
};