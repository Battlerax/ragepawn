#include "pawn.hpp"

namespace fs = std::experimental::filesystem;

Pawn::Pawn()
{
	std::cout << "Implementing best lang ever created.... )))))))" << std::endl;
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	std::string path = std::string(buffer).substr(0, pos);
	path.append("\\script.amx");
	if (InitializeAMX(path))
	{
		Sleep(5000);
		RegisterNatives();
		CallMain();
	}
	else printf("Failed to load script entirely.... (((((((((((\n");
}

int Pawn::CallMain()
{
	cell ret;
	int e = amx_Exec(&amx, &ret, AMX_EXEC_MAIN);
	std::cout << "fuck p" << std::endl;
	if (e != AMX_ERR_NONE)
	{
		ScriptError();
		return false;
	}
	std::cout << "fuck life" << std::endl;
	if (ret != 0) printf("Returns %ld\n", (long)ret);
	return true;
}

void Pawn::ScriptError()
{
	printf("Run time error %d: \"%s\"\n", err, aux_StrError(err));
}

void Pawn::KillScript()
{
	std::cout << "Killing script..." << std::endl;
	amx_Cleanup(&amx);
	if (script) free(script);
	script = NULL;
}

bool Pawn::LoadAMX(char * file, void * program)
{
	err = aux_LoadProgram(&amx, file, program);
	if (err != AMX_ERR_NONE)
	{
		ScriptError();
		return false;
	}
	return true;
}

static cell n_verify(AMX *amx, const cell *params)
{
	Natives::verify();
	return 1;
}

const AMX_NATIVE_INFO print_Natives[] =
{
	{ "verify", n_verify },
	{ NULL, NULL }
};


bool Pawn::RegisterNatives()
{
	/*extern AMX_NATIVE_INFO console_Natives[];
	extern AMX_NATIVE_INFO core_Natives[];
	err = amx_Register(&amx, console_Natives, -1);
	if (err != AMX_ERR_NONE)
	{
		ScriptError();
		return false;
	}
	err = amx_Register(&amx, core_Natives, -1);
	if (err != AMX_ERR_NONE)
	{
		ScriptError();
		return false;
	}*/
	err = amx_Register(&amx, print_Natives, -1);
	if (err != AMX_ERR_NONE)
	{
		ScriptError();
		return false;
	}

	cell num = -1;
	amx_NumNatives(&amx, &num);
	printf("Used %i native functions.\n", num);
	return true;
}

void Pawn::SetMultiplayer(rage::IMultiplayer *mp)
{
	this->m_mp = mp;
	mp->AddEventHandler(dynamic_cast<rage::IEventHandler*>(&GetEventInstance()));
}



int Pawn::InitializeAMX(const std::string& path)
{
	std::cout << path << std::endl;
	long memsize = aux_ProgramSize(_strdup(path.c_str()));
	if (memsize == 0)
	{
		printf("Script file not found or corrupted\n");
		return 0;
	}

	void * program = malloc(memsize);
	if (program == 0)
	{
		printf("Memory allocation for script failed\n");
		return 0;
	}

	if (!LoadAMX(_strdup(path.c_str()), program))
	{
		printf("Loading script into Abstract Machine failed\n");
		return 0;
	}
	return 1;
}

