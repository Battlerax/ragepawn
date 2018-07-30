#include <stdio.h>
#define TYPEDEF_INT
#include "../amx32/amxaux.h"
#include "../amx32/amx.h"


void ErrorExit(AMX *amx, int errorcode)
{
	printf("Run time error %d: \"%s\" on address %ld\n",
		errorcode, aux_StrError(errorcode),
		(amx != NULL) ? amx->cip : 0);
	exit(1);
}

void print_int(int value)
{
	printf("SCRIPT VALUE: %i\n", value);
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


int main(int argc, char *argv[])
{
	AMX amx;
	cell ret = 0;
	int err;

	err = aux_LoadProgram(&amx, "script.amx", NULL);
	printf("%d\n", err);
	if (err != AMX_ERR_NONE) ErrorExit(&amx, err);

	err = amx_Register(&amx, print_Natives, -1);
	if (err) ErrorExit(&amx, err);

	err = amx_Exec(&amx, &ret, AMX_EXEC_MAIN);
	if (err) ErrorExit(&amx, err);

	printf("%s returns %ld\n", argv[1], (long)ret);
	aux_FreeProgram(&amx);
	_sleep(3000);
	return 0;
}