#include "main.hpp"
#include "format.hpp"
#include "pawn.hpp"

// native GetHashKey(const string[]);
NATIVE (n_joaat)
{
	GET_STRING(params[1], 128);
	return Pawn::joaat(output);
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