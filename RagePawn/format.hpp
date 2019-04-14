#pragma once
#include "main.hpp"
#include "../amxlib/amx.h"

typedef struct {
	const cell *params;
	int numparams;
	int skip;
	int length;
} strdata;

enum {
	SV_DECIMAL,
	SV_HEX
};

enum {
	FMT_NONE,   /* not in format state; accept '%' */
	FMT_START,  /* found '%', accept '+', '-' (START), '0' (filler; START), digit (WIDTH), '.' (DECIM), or '%' or format letter (done) */
	FMT_WIDTH,  /* found digit after '%' or sign, accept digit (WIDTH), '.' (DECIM) or format letter (done) */
	FMT_DECIM,  /* found digit after '.', accept accept digit (DECIM) or format letter (done) */
};

cell AMX_NATIVE_CALL n_format(AMX *amx, const cell *params);
std::string get_str(AMX *amx, cell *cstr, strdata *info);
static std::string to_string(AMX *amx, TCHAR ch, cell param, TCHAR sign, TCHAR decpoint, int width, int digits, TCHAR filler);
static int formatstate(TCHAR c, int *state, TCHAR *sign, TCHAR *decpoint, int *width, int *digits, TCHAR *filler);
static TCHAR *amx_strval(TCHAR buffer[], long value, int format, int width);
static TCHAR *reverse(TCHAR *string, int stop);