#include "main.hpp"
#include "format.hpp"

AMX_NATIVE_INFO util_Natives[] =
{
	{ "format", n_format },
	{ NULL, NULL }
};

int amx_utilInit(AMX *amx)
{
	return amx_Register(amx, util_Natives, -1);
}