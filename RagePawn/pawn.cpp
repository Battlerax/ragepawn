#include "pawn.hpp"
#include "PlayerNatives.hpp"

#include "../amxlib/amx.h"
#include "../amxlib/amxaux.h"

#include <cstdarg>

namespace fs = std::experimental::filesystem;

extern "C" {
	int AMXAPI amx_ConsoleInit(AMX *amx);
	int AMXAPI amx_ConsoleCleanup(AMX *amx);
	int AMXAPI amx_CoreInit(AMX *amx);
	int AMXAPI amx_CoreCleanup(AMX *amx);
}

std::string Last(std::string const& str, std::string const& delimiter) {
	return str.substr(str.find_last_of(delimiter) + delimiter.size());
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
		const auto filename = Last(path_str, "\\");
		if (filename.find(".amx") == std::string::npos) continue;

		std::cout << "Loading '" + filename + "'.." << std::endl;
		RunAMX(path_str);
		std::cout << std::endl;
	}
}

void print_int_d(int value) 
{
	std::cout << value << std::endl;
}

static cell AMX_NATIVE_CALL n_printd(AMX * amx, const cell * params)
{
	//std::cout << (int)params[1] << std::endl;
	print_int_d((int)params[1]);
	return 0;
}

const AMX_NATIVE_INFO rage_Natives[] =
{
	{ "printd", n_printd },
	{ "print_str", PlayerNatives::n_print_str },
	{ "GetPlayerName", PlayerNatives::n_GetPlayerName },
	{ NULL, NULL }
};


int Pawn::RunAMX(const std::string& path)
{
	const auto path_str = path.c_str();
	cell ret = 0;
	int num = 0;

	// LoadProgram
	err = aux_LoadProgram(&amx, (char*)path_str, NULL);
	if (err != AMX_ERR_NONE) return Terminate();

	// LoadNatives 
	amx_ConsoleInit(&amx);
	//err = amx_CoreInit(&amx);
	//if (err != AMX_ERR_NONE) return Terminate();

	err = amx_Register(&amx, rage_Natives, -1);
	if (err != AMX_ERR_NONE) return Terminate();

	amx_NumNatives(&amx, &num);
	std::cout << "Registered natives: " << num << std::endl;

	// ExecProgram
	err = amx_Exec(&amx, &ret, AMX_EXEC_MAIN);
	if (err != AMX_ERR_NONE) return Terminate();

	std::cout << "Script has returned: " << ret << std::endl;

	//aux_FreeProgram(&amx);
	return 1;
}

int Pawn::Terminate() const
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

// TODO: better error handling needed here....
void Pawn::Callback(const char *name, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	
	int index;
	err = amx_FindPublic(&amx, name, &index);
	if (err != AMX_ERR_NONE) 
	{
		Terminate();
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
		/*else if(*fmt == 'f') 
		{
			float f = va_arg(args, float);
		}*/
		++fmt;
	}

	err = amx_Exec(&amx, NULL, index);
	if (err != AMX_ERR_NONE) Terminate();

	for (auto &i : addresses) {
		amx_Release(&amx, i);
		i = NULL;
	}
	addresses.clear();
	

	va_end(args);

	std::cout << "Finished callback..." << std::endl;
}