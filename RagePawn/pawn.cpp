#include "pawn.hpp"
#include "../amxlib/amx.h"
#include "../amxlib/amxaux.h"

namespace fs = std::experimental::filesystem;

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

void print_int(int value)
{
	printf("%i\n", value);
}

static cell AMX_NATIVE_CALL n_print_int(AMX *amx, const cell *params)
{
	print_int((int)params[1]);
	return 0;
}

const AMX_NATIVE_INFO print_Natives[] =
{
	{ "print_int", n_print_int },
	{ NULL, NULL }
};

int Pawn::RunAMX(const std::string& path)
{
	auto path_str = path.c_str();
	cell ret = 0;
	int num = 0;

	// LoadProgram
	err = aux_LoadProgram(&amx, (char*)path_str, NULL);
	if (err != AMX_ERR_NONE) return Terminate();

	// Load Natives
	err = amx_Register(&amx, print_Natives, -1);
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

int Pawn::Terminate()
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
	mp->AddEventHandler(dynamic_cast<rage::IEventHandler*>(&GetEventInstance()));
}