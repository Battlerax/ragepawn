#include "pawn.hpp"

#include "../amxlib/amxaux.h" 

#include <filesystem> 
#include "format.hpp"

#pragma comment( lib, "winmm.lib")  // amx_TimeInit(&amx); 
#pragma comment( lib, "ws2_32.lib")  // amx_DGramInit(&amx); 

#include "callbacks.hpp"

namespace fs = std::experimental::filesystem;

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

int amx_playerInit(AMX *amx);
int amx_utilInit(AMX *amx);

Pawn::Pawn()
{
	std::cout << "Initializing RagePawn.." << std::endl;
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	const std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	std::string path = std::string(buffer).substr(0, pos);
	Iterate(path.append("\\amx\\filterscripts"), true);
	path.clear();
	std::cout << std::endl;
	Iterate(path.append("\\amx\\gamemodes"), false);
}

void Pawn::Iterate(const std::string& path, const bool fs)
{
	std::cout << "-> Loading " << path.substr(path.find_last_of("\\/") + 1) << ".." << std::endl;
	for (const auto & p : fs::directory_iterator(path))
	{
		const auto path_str = p.path().string();
		const auto filename = path_str.substr(path_str.find_last_of("\\/") + 1);
		if (filename.substr(filename.find_last_of('.') + 1) != "amx") continue;

		std::cout << "--> Loading '" << filename << "'.." << std::endl;
		RunAMX(path_str, fs);
	}
}

void Pawn::RunAMX(const std::string& path, const bool fs)
{
	AMX amx;

	const auto path_str = path.c_str();
	const int err = aux_LoadProgram(&amx, (char*)path_str, NULL);
	if (err != AMX_ERR_NONE) TerminateLoad(path.substr(path.find_last_of("\\/") + 1));

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

	amx_utilInit(&amx);
	amx_playerInit(&amx);
	

	//int count;
	//amx_NumNatives(&amx, &count);
	//printf("natives: %d\n", count);
	//for (int i = 0; i < count; i++) {
	//    char temp[32];
	//    amx_GetNative(&amx, i, temp);
	//    printf("     %s\n", temp);
	//}

	//amx_NumPublics(&amx, &count);
	//printf("publics: %d\n", count);
	//for (int i = 0; i < count; i++) {
	//    char temp[32];
	//    amx_GetPublicEx(&amx, i, temp);
	//    printf("     %s\n", temp);
	//}

	amx_Exec(&amx, nullptr, AMX_EXEC_MAIN); // execute main

	script scr;
	scr.amx = amx;
	scr.fs = fs;
	scripts.push_back(scr);
	fs ? CallPublic(&amx, "OnFilterScriptInit") : CallPublic(&amx, "OnGameModeInit");
}

//void Pawn::UnloadAMX()
//{
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
//}

int Pawn::TerminateLoad(const std::string& filename)
{
	std::cout << "Error: Failed to load '" << filename << "'.." << std::endl;
	return false;
}

int Pawn::Terminate(const int err)
{
	printf("Run time error %d: \"%s\"\n", err, aux_StrError(err));
	exit(1);
}

void Pawn::TerminateScript(AMX *amx)
{
	std::cout << "Terminating script..." << std::endl;
	amx_Cleanup(amx);
	aux_FreeProgram(amx);
}

void Pawn::SetMultiplayer(rage::IMultiplayer *mp)
{
	this->m_mp = mp;
	mp->AddEventHandler(dynamic_cast<rage::IEventHandler*>(&EventHandler::GetInstance()));
}

void Pawn::CallPublic(AMX *amx, const char* name)
{
	int id;
	if (!amx_FindPublic(amx, name, &id))
		amx_Exec(amx, nullptr, id);
}

void Pawn::CallPublicEx(AMX *amx, const char *name, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	
	int index;
	int err = amx_FindPublic(amx, name, &index);
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
			amx_Push(amx, (cell)va_arg(args, int));
		}
		else if (*fmt == 's') 
		{
			const char * s = va_arg(args, const char*);
			cell* address;
			amx_PushString(amx, &address, s, 0, 0);
			addresses.push_back(address);
		}
		else if(*fmt == 'f') 
		{
			float f = va_arg(args, float);
		}
		++fmt;
	}

	err = amx_Exec(amx, NULL, index);
	if (err != AMX_ERR_NONE) Terminate(err);

	for (auto &i : addresses) {
		amx_Release(amx, i);
		i = NULL;
	}
	addresses.clear();
	va_end(args);

	std::cout << "Finished callback..." << std::endl;
}

int Pawn::joaat(std::string string)
{
	size_t i = 0;
	int32_t hash = 0;
	std::transform(string.begin(), string.end(), string.begin(), ::tolower);
	while (i != string.length()) {
		hash += string[i++];
		hash += hash << 10;
		hash ^= hash >> 6;
	}
	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;
	return hash;
}

int Pawn::str_cell_size(const cell* param)
{
	int j = sizeof(cell) - sizeof(char);
	int count = 0;
	int i = 0;
	for (; ; )
	{
		count++; // count the last null
		const auto c = (char)((ucell)param[i] >> 8 * j);
		if (c == 0) break;
		if (j == 0) i++;
		j = (j + sizeof(cell) - sizeof(char)) % sizeof(cell);
	}
	return count;
}
