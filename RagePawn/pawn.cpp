#include "pawn.hpp"
#include "PlayerNatives.hpp"

#include "../amxlib/amxaux.h" 

#include <filesystem> 
#include "format.hpp"

#pragma comment( lib, "winmm.lib")  // amx_TimeInit(&amx); 
#pragma comment( lib, "ws2_32.lib")  // amx_DGramInit(&amx); 

namespace fs = std::experimental::filesystem;

std::string last(std::string const& str, std::string const& delimiter) { return str.substr(str.find_last_of(delimiter) + delimiter.size()); }

extern "C" {
	int AMXEXPORT AMXAPI amx_ConsoleInit(AMX *amx);
	int AMXEXPORT AMXAPI amx_ConsoleCleanup(AMX *amx);
	int AMXEXPORT AMXAPI amx_StringInit(AMX *amx);
	int AMXEXPORT AMXAPI amx_StringCleanup(AMX *amx);
	int AMXEXPORT AMXAPI amx_CoreInit(AMX *amx);
	int AMXEXPORT AMXAPI amx_CoreCleanup(AMX *amx);
	int AMXEXPORT AMXAPI amx_DGramInit(AMX *amx);
	int AMXEXPORT AMXAPI amx_DGramCleanup(AMX *amx);
	int AMXEXPORT AMXAPI amx_FileInit(AMX *amx);
	int AMXEXPORT AMXAPI amx_FileCleanup(AMX *amx);
	int AMXEXPORT AMXAPI amx_FloatInit(AMX *amx);
	int AMXEXPORT AMXAPI amx_FloatCleanup(AMX *amx);
	int AMXEXPORT AMXAPI amx_FixedInit(AMX *amx);
	int AMXEXPORT AMXAPI amx_FixedCleanup(AMX *amx);
	int AMXEXPORT AMXAPI amx_TimeInit(AMX *amx);
	int AMXEXPORT AMXAPI amx_TimeCleanup(AMX *amx);
	int AMXEXPORT AMXAPI amx_ProcessInit(AMX *amx);
	int AMXEXPORT AMXAPI amx_ProcessCleanup(AMX *amx);
	int AMXEXPORT AMXAPI amx_ArgsInit(AMX *amx);
	int AMXEXPORT AMXAPI amx_ArgsCleanup(AMX *amx);
}

Pawn::Pawn()
{
	memset(&amx, 0, sizeof(amx));
	std::cout << "Initializing RagePawn.." << std::endl;
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	std::string path = std::string(buffer).substr(0, pos);
	path.append("\\amx\\");

	for (const auto & p : fs::directory_iterator(path))
	{
		const auto path_str = p.path().string();
		const auto filename = last(path_str, "\\");
		if (filename.find(".amx") == std::string::npos) continue;

		std::cout << "Loading '" + filename + "'.." << std::endl;
		RunAMX(path_str);
		std::cout << std::endl;
	}
}

const AMX_NATIVE_INFO rage_Natives[] =
{
	{ "GetPlayerName", PlayerNatives::n_GetPlayerName },
	{ "format", n_format},
	{ NULL, NULL }
};

int Pawn::RunAMX(const std::string& path)
{
	const auto path_str = path.c_str();
	cell ret = 0;
	int num = 0;

	// LoadProgram
	err = aux_LoadProgram(&amx, (char*)path_str, NULL);
	if (err != AMX_ERR_NONE) return Terminate(err);

	// LoadNatives 
	amx_ConsoleInit(&amx);
	amx_StringInit(&amx);
	amx_CoreInit(&amx);
	amx_DGramInit(&amx);
	amx_FloatInit(&amx);
	amx_FileInit(&amx);
	amx_FixedInit(&amx);
	amx_TimeInit(&amx);
	amx_ProcessInit(&amx);
	amx_ArgsInit(&amx);

	amx_Register(&amx, rage_Natives, -1);

	int count;
	amx_NumNatives(&amx, &count);
	printf("natives: %d\n\r", count);
	for (int i = 0; i < count; i++) {
	    char temp[32];
	    amx_GetNative(&amx, i, temp);
	    printf("     %s\n\r", temp);
	}

	amx_NumPublics(&amx, &count);
	printf("publics: %d\n\r", count);
	for (int i = 0; i < count; i++) {
	    char temp[32];
	    amx_GetPublicEx(&amx, i, temp);
	    printf("     %s\n\r", temp);
	}

	// ExecProgram
	err = amx_Exec(&amx, &ret, AMX_EXEC_MAIN);
	if (err != AMX_ERR_NONE) return Terminate(err);

	aux_FreeProgram(&amx);
	return 1;
}

void UnloadAMX()
{
    //amx_ArgsCleanup();
    //amx_ProcessCleanup();
    //amx_TimeCleanup();
    //amx_FixedCleanup();
    //amx_FileCleanup();
    //amx_FloatCleanup();
    //amx_DGramCleanup();
    //amx_CoreCleanup();
    //amx_StringCleanup();
    //amx_ConsoleCleanup();
    
}

int Pawn::Terminate(const int err)
{
	std::cout << "Terminating.." << std::endl;
	printf("Run time error %d: \"%s\"\n", err, aux_StrError(err));
	return false;
}

void Pawn::TerminateScript()
{
	std::cout << "Terminating script..." << std::endl;
	amx_Cleanup(&amx);
	aux_FreeProgram(&amx);
}

void Pawn::SetMultiplayer(rage::IMultiplayer *mp)
{
	this->m_mp = mp;
	mp->AddEventHandler(dynamic_cast<rage::IEventHandler*>(&gm::EventHandler::GetInstance()));
}

void Pawn::Callback(const char *name, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	
	int index;
	err = amx_FindPublic(&amx, name, &index);
	if (err != AMX_ERR_NONE) 
	{
		Terminate(err);
		return;
	}

	std::vector<cell*> addresses;

	while (*fmt != '\0') 
	{
		if (*fmt == 'd') 
		{
			amx_Push(&amx, (cell)va_arg(args, int));
		}
		else if (*fmt == 's') 
		{
			const char * s = va_arg(args, const char*);
			cell* address;
			amx_PushString(&amx, &address, s, 0, 0);
			addresses.push_back(address);
		}
		else if(*fmt == 'f') 
		{
			float f = va_arg(args, float);
		}
		++fmt;
	}

	err = amx_Exec(&amx, NULL, index);
	if (err != AMX_ERR_NONE) Terminate(err);

	for (auto &i : addresses) {
		amx_Release(&amx, i);
		i = NULL;
	}
	addresses.clear();
	va_end(args);

	std::cout << "Finished callback..." << std::endl;
}