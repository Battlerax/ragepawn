// amxTester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>     /* for exit() */
#include <string.h> /* for memset() (on some compilers) */

#define CDECL
#include "../amxlib/amx.h"
#include "../amxlib/amxaux.h"


void ErrorExit(AMX *amx, int errorcode)
{
	printf("Run time error %d: \"%s\" on address %ld\n",
		errorcode, aux_StrError(errorcode),
		(amx != NULL) ? amx->cip : 0);
	exit(1);
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


int main(int argc, char *argv[])
{
	AMX amx;
	cell ret = 0;
	int err;

	err = aux_LoadProgram(&amx, "script.amx", NULL);
	if (err != AMX_ERR_NONE) ErrorExit(&amx, err);

	printf("1");

	err = amx_Register(&amx, print_Natives, -1);
	if (err) ErrorExit(&amx, err);

	printf("2");

	err = amx_Exec(&amx, &ret, AMX_EXEC_MAIN);
	if (err) ErrorExit(&amx, err);

	printf("3");
	
	printf("%s returns %ld\n", argv[1], (long)ret);
	aux_FreeProgram(&amx);
	_sleep(3000);
    return 0;
}

