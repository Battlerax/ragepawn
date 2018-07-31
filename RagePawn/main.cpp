#include "main.hpp"
#include "pawn.hpp"
#include "../ragemp-cppsdk/rage.hpp" 

BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
			// Initialize once for each new process.
			// Return FALSE to fail DLL load.
			break;

		case DLL_PROCESS_DETACH:
			Pawn::GetInstance().TerminateScript();
			break;
	}
	return TRUE;  // Successful DLL_PROCESS_ATTACH.
}

RAGE_API rage::IPlugin *InitializePlugin(rage::IMultiplayer *mp)
{
	Pawn::GetInstance().SetMultiplayer(mp);
	return new rage::IPlugin;
}