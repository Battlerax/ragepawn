#include "main.hpp"
#include "format.hpp"
#include "pawn.hpp"

// native GetHashKey(const string[]);
static cell AMX_NATIVE_CALL n_joaat(AMX *amx, const cell *params)
{
	char str[512];
	cell* cstr = amx_Address(amx, params[1]);
	amx_GetString(str, cstr, 0, sizeof str);
	return Pawn::joaat(str);
}

const AMX_NATIVE_INFO util_Natives[] =
{
	{ "format", n_format },
	{ "GetHashKey", n_joaat },
	{ NULL, NULL }
};

int amx_utilInit(AMX *amx)
{
	return amx_Register(amx, util_Natives, -1);
}