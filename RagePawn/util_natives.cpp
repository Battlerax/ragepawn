#include "main.hpp"
#include "format.hpp"
#include "pawn.hpp"

// native GetHashKey(const string[]);
NATIVE (n_joaat)
{
	char* func;
	GET_STRING(amx, params[1], func);
	return Pawn::joaat(func);
}

// native println(const string[]);
NATIVE (n_println)
{
	char* pStr;
	GET_STRING(amx, params[1], pStr);

	std::cout << params[1] << std::endl;
	return 0;
}

// native printfln(const format[], { Float,Fixed,_ }:...);
NATIVE(n_printfln)
{
	cell *cstr;
	strdata info;

	memset(&info, 0, sizeof info);
	info.params = params + 2;
	info.numparams = (int)(params[0] / sizeof(cell)) - 1;
	info.skip = 0;
	info.length = INT_MAX;

	cstr = amx_Address(amx, params[1]);
	std::cout << get_str(amx, cstr, &info) << std::endl;
	return 0;
}

const AMX_NATIVE_INFO util_Natives[] =
{
	{ "format", n_format },
	{ "GetHashKey", n_joaat },
	{ "println", n_println },
	{ "printfln", n_printfln },
	{ NULL, NULL }
};

int amx_utilInit(AMX *amx)
{
	return amx_Register(amx, util_Natives, -1);
}