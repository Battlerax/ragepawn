#include "format.hpp"
#include <tchar.h>
#include <cassert>

// native format(output[], size = sizeof output, const format[], ...);
cell AMX_NATIVE_CALL n_format(AMX *amx, const cell *params)
{
	strdata info;
	info.params = params + 4;
	info.numparams = (int)(params[0] / sizeof(cell) - 3);
	info.skip = 0;
	info.length = (int)params[2];

	cell* cstra = amx_Address(amx, params[3]);
	const auto result = get_str(amx, cstra, &info) + '\0';

	SET_STRING(result.c_str(), params[1], params[2]);
	return true;
}

static std::string to_string(AMX *amx, TCHAR ch, cell param, TCHAR sign, TCHAR decpoint, int width, int digits, TCHAR filler)
{
	cell *cptr; TCHAR buffer[40];
	std::string end_result;

#if defined FLOATPOINT
	TCHAR formatstring[40];
#endif

#if !defined FIXEDPOINT && !defined FLOATPOINT
	(void)decpoint;
#endif

	switch (ch)
	{
		case __T('c'):
		{
			cptr = amx_Address(amx, param);
			width--; /* single character itself has a with of 1 */
			if (sign != __T('-'))
			{
				while (width-->0)
				{
					end_result += filler;
				}
			}
			end_result += (TCHAR)*cptr;
			while (width-->0)
			{
				end_result += filler;
			}
			return end_result;
		}

		case __T('d'):
		{
			cell value;
			int length = 1;
			cptr = amx_Address(amx, param);
			value = *cptr;
			if (value<0 || sign == __T('+'))
			{
				length++;
			}
			if (value<0)
			{
				value = -value;
			}
			while (value >= 10)
			{
				length++;
				value /= 10;
			}
			width -= length;
			if (sign != __T('-'))
			{
				while (width-->0)
				{
					end_result += filler;
				}
			}
			amx_strval(buffer, *cptr, 0, 0);
			if (sign == __T('+') && *cptr >= 0)
			{
				end_result += sign;
			}
			end_result += buffer;
			while (width-->0)
			{
				end_result += filler;
			}
			return end_result;
		} /* case */

	#if defined FLOATPOINT
		case __T('f'): /* 32-bit floating point number */
		case __T('r'): /* if floating point is enabled, %r == %f */
					   /* build a format string */
			if (digits == INT_MAX)
				digits = 5;
			else if (digits>25)
				digits = 25;
			_tcscpy(formatstring, __T("%"));
			if (sign != __T('\0'))
				_stprintf(formatstring + _tcslen(formatstring), __T("%c"), sign);
			if (width>0)
				_stprintf(formatstring + _tcslen(formatstring), __T("%d"), width);
			_stprintf(formatstring + _tcslen(formatstring), __T(".%df"), digits);
			cptr = amx_Address(amx, param);
	#if PAWN_CELL_SIZE == 64
			_stprintf(buffer, formatstring, *(double*)cptr);
	#else
			_stprintf(buffer, formatstring, *(float*)cptr);
	#endif
			if (decpoint == __T(',')) {
				TCHAR *ptr = _tcschr(buffer, __T('.'));
				if (ptr != NULL)
					*ptr = __T(',');
			} /* if */
			f_putstr(user, buffer);
			return 1;
	#endif

	#if defined FIXEDPOINT
	#define FIXEDMULT 1000
		case __T('q'): /* 32-bit fixed point number */
	#if !defined FLOATPOINT
		case __T('r'): /* if fixed point is enabled, and floating point is not, %r == %q */
	#endif
			cptr = amx_Address(amx, param);
			/* format the number */
			if (digits == INT_MAX)
				digits = 3;
			else if (digits>25)
				digits = 25;
			formatfixed(buffer, *cptr, sign, width, decpoint, digits, filler);
			assert(_tcslen(buffer)<sizeof buffer);
			f_putstr(user, buffer);
			return 1;
	#endif

	#if !defined FLOATPOINT && !defined FIXEDPOINT
		case __T('f'):
		case __T('q'):
		case __T('r'):
			//f_putstr(user, __T("(no rational number support)"));
			end_result += "(no rational number support)";
			return end_result; /* flag this as an error */
	#endif

		case __T('s'):
		{
			strdata info;
			memset(&info, 0, sizeof info);
			info.length = digits;
			cptr = amx_Address(amx, param);
			return get_str(amx, cptr, &info);
		}

		case __T('x'):
		{
			ucell value;
			int length = 1;
			cptr = amx_Address(amx, param);
			value = *(ucell*)cptr;
			while (value >= 0x10)
			{
				length++;
				value >>= 4;
			}
			width -= length;
			if (sign != __T('-'))
			{
				while (width-->0)
				{
					end_result += filler;
				}
			}
			amx_strval(buffer, (long)*cptr, 1, 0);
			end_result += buffer;
			while (width-->0)
			{
				end_result += filler;
			}
			return end_result;
		}
	}
	end_result += ch;
	return end_result;
}

static std::string get_str(AMX *amx, cell *cstr, strdata *info)
{
	int i, paramidx = 0;
	int fmtstate = FMT_NONE, width, digits;
	TCHAR sign, decpoint, filler;
	std::string end_result;

	/* check whether this is a packed string */
	if ((ucell)*cstr > UNPACKEDMAX) /* the string is packed */
	{
		int j = sizeof(cell) - sizeof(char);
		i = 0;
		for (; ; )
		{
			const auto c = (char)((ucell)cstr[i] >> 8 * j);
			if (c == 0) break;
			switch (formatstate(c, &fmtstate, &sign, &decpoint, &width, &digits, &filler))
			{
				case -1:
				{
					end_result += c;
					break;
				}

				case 0: break;

				case 1:
				{
					assert(info != NULL && info->params != NULL);
					if (paramidx >= info->numparams)  /* insufficient parameters passed */
					{
						amx_RaiseError(amx, AMX_ERR_NATIVE);
					}
					else
					{
						end_result += to_string(amx, c, info->params[paramidx], sign, decpoint, width, digits, filler);
						if (!end_result.empty()) paramidx++;
					}
					fmtstate = FMT_NONE;
					break;
				}

				default: assert(0);
			} /* switch */

			if (j == 0) i++;
			j = (j + sizeof(cell) - sizeof(char)) % sizeof(cell);
		} /* for */
	}
	else
	{
		/* the string is unpacked */
		for (i = 0; cstr[i] != 0; i++)
		{
			switch (formatstate((TCHAR)cstr[i], &fmtstate, &sign, &decpoint, &width, &digits, &filler))
			{
				case -1:
				{
					end_result += (TCHAR)cstr[i];
					break;
				}

				case 0: break;

				case 1:
				{
					assert(info != NULL && info->params != NULL);
					if (paramidx >= info->numparams)  /* insufficient parameters passed */
					{
						amx_RaiseError(amx, AMX_ERR_NATIVE);
					}
					else
					{
						end_result += to_string(amx, (TCHAR)cstr[i], info->params[paramidx], sign, decpoint, width, digits, filler);
						if (!end_result.empty()) paramidx++;
					}
					fmtstate = FMT_NONE;
					break;
				}

				default: assert(0);
			} /* switch */
		} /* for */
	} /* if */

	return end_result;
}

static int formatstate(TCHAR c, int *state, TCHAR *sign, TCHAR *decpoint, int *width, int *digits, TCHAR *filler)
{
	assert(state != NULL && sign != NULL && decpoint != NULL && width != NULL && digits != NULL && filler != NULL);
	switch (*state) {
	case FMT_NONE:
		if (c == __T('%')) {
			*state = FMT_START;
			*sign = __T('\0');
			*decpoint = __T('.');
			*width = 0;
			*digits = INT_MAX;
			*filler = __T(' ');
		}
		else {
			return -1;  /* print a single character */
		} /* if */
		break;
	case FMT_START:
		if (c == __T('+') || c == __T('-')) {
			*sign = c;
		}
		else if (c == __T('0')) {
			*filler = c;
		}
		else if (c >= __T('1') && c <= __T('9')) {
			*width = (int)(c - __T('0'));
			*state = FMT_WIDTH;
		}
		else if (c == __T('.') || c == __T(',')) {
			*decpoint = c;
			*digits = 0;
			*state = FMT_DECIM;
		}
		else if (c == __T('%')) {
			*state = FMT_NONE;
			return -1;  /* print literal '%' */
		}
		else {
			return 1;   /* print formatted character */
		} /* if */
		break;
	case FMT_WIDTH:
		if (c >= __T('0') && c <= __T('9')) {
			*width = *width * 10 + (int)(c - __T('0'));
		}
		else if (c == __T('.') || c == __T(',')) {
			*decpoint = c;
			*digits = 0;
			*state = FMT_DECIM;
		}
		else {
			return 1;   /* print formatted character */
		} /* if */
		break;
	case FMT_DECIM:
		if (c >= __T('0') && c <= __T('9')) {
			*digits = *digits * 10 + (int)(c - __T('0'));
		}
		else {
			return 1;   /* print formatted character */
		} /* if */
		break;
	} /* switch */

	return 0;
}

static TCHAR *amx_strval(TCHAR buffer[], long value, int format, int width)
{
	int start, stop;
	TCHAR temp;

	start = stop = 0;
	if (format == SV_DECIMAL) {
		if (value < 0) {
			buffer[0] = __T('-');
			start = stop = 1;
			do {
				temp = (TCHAR)(value % 10);
				if (temp > 0)
					temp = (TCHAR)(temp - 10);
				buffer[stop++] = (TCHAR)(__T('0') - temp);
				value /= 10;
			} while (value != 0);
		}
		else {
			do {
				buffer[stop++] = (TCHAR)((value % 10) + __T('0'));
				value /= 10;
			} while (value != 0);
		}
	}
	else {
		/* hexadecimal */
		unsigned long v = (unsigned long)value;	/* copy to unsigned value for shifting */
		do {
			buffer[stop] = (TCHAR)((v & 0x0f) + __T('0'));
			if (buffer[stop] > __T('9'))
				buffer[stop] += (TCHAR)(__T('A') - __T('0') - 10);
			v >>= 4;
			stop++;
		} while (v != 0);
	} /* if */

	  /* pad to given width */
	if (width < 0) {
		temp = __T('0');
		width = -width;
	}
	else {
		temp = __T(' ');
	} /* if */
	while (stop < width)
		buffer[stop++] = temp;

	buffer[stop] = __T('\0');

	/* swap the string, and we are done */
	reverse(buffer + start, stop - start);
	return buffer;
}

static TCHAR *reverse(TCHAR *string, int stop)
{
	int start = 0;
	TCHAR temp;

	/* swap the string */
	stop--;				/* avoid swapping the '\0' byte to the first position */
	while (stop - start > 0) {
		temp = string[start];
		string[start] = string[stop];
		string[stop] = temp;
		start++;
		stop--;
	} /* while */
	return string;
}