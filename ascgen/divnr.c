#pragma once
#include <stdlib.h>
#include "multrob.h"
#include "stdextension.h"

static void cleannums(struct multdata* d)
{
	free(d->num1);
	free(d->num2);
	free(d->result);
	free(d->reminder);
	free(d->correction);
}

static void add_correction(struct multdata d)
{
	int len = max_i(strlen_i(d.result), strlen_i(d.correction));
	d.result[len] = '\0';

	int carry = 0;
	for (int i = len - 1; i >= 0; i--)
	{
		if (d.result[i] == ',')
			continue;

		int sum = carry;
		sum += str_value_at(d.result, i);
		sum += str_value_at(d.correction, i);

		d.result[i] = ascvalue_to_char(sum % 2);
		carry = sum / 2;
	}
}

static void printdiv(FILE* stream, struct multdata d, int precision)
{
	mathmult_shift_left(d.num1, d.shift1);
	mathmult_shift_left(d.num2, d.shift2);

	fprintf(stream, "X*   = X_d * 2^(%d) = %s\n", -d.shift1, d.num1);
	fprintf(stream, "Y*   = Y_d * 2^(%d) = %s\n\n", -d.shift2, d.num2);

	int begin_offset = 7, line_width = d.numlen + 2;
	fprintf(stream, "X*   = %s\n", d.num1);
	fprintf(stream, "Y*   = %s\n", d.num2);

	char* yplus = d.num2;
	char* yminus = malloc_s(strlen_i(yplus) + 1);
	str_compliment_Pm1(yplus, yminus, 2);
	strcpy(d.reminder, d.num1);

	int divlen = precision == 0 ? d.numlen : precision;
	d.result[divlen] = '\0';
	for(int i = 0; i < divlen; i++)
	{
		char* rem_expl = i == 0 ? " = X*  " : "       ";
		char zi = d.reminder[0] == yplus[0] ? '1' : '0';
		char* eq = zi == '1' ? " =" : "!=";
		char* addY = zi == '1' ? yminus : yplus;
		d.result[i] = zi;

		example_printline(stream, begin_offset, line_width);
		fprintf(stream, "%*s%s  r%d%s",begin_offset, "", d.reminder, i, rem_expl);
		fprintf(stream, "sgn r%d %s sgn Y -> z'%d = %c\n", i, eq, i, zi);

		str_addnum(d.reminder, d.reminder, d.reminder, 2);
		fprintf(stream, "%*s%c%s  2r%d\n", begin_offset - 1, "", 179, d.reminder, i);
		fprintf(stream, "%*s+%c%s  %cY*\n", begin_offset - 2, "", 179, addY, zi == '1' ? '-' : '+');
		str_addnum(d.reminder, addY, d.reminder, 2);
	}

	example_printline(stream, begin_offset, line_width);
	fprintf(stream, "%*s%s  r%d\n\n", begin_offset, "", d.reminder, divlen);

	fprintf(stream, "R%d = r%d * 2^(%d) = %s * 2^(%d) = ", divlen, divlen, -divlen, d.reminder, -divlen);
	str_expand(d.reminder, 1 + divlen, 0, d.reminder[0]);
	mathmult_shift_left(d.reminder, divlen);
	fprintf(stream, "%s\n\n", d.reminder);
	
	char corr_desc[20];
	begin_offset = sprintf(corr_desc, "1+2^(%d)", -divlen);
	strcat_b(&d.result[1], ",");
 	line_width = sprintf(d.correction, "1,%*s", divlen, "1");
	str_pad_space(d.correction, '0');

	fprintf(stream, "%-*s = %s\n",begin_offset, "Z'", d.result);
	fprintf(stream, "%s = %s +\n", corr_desc, d.correction);
	example_printline(stream, begin_offset + 3, line_width);
	add_correction(d);
	fprintf(stream, "%-*s = %s\n", begin_offset, "Z*", d.result);
	fprintf(stream, "%-*s = %s\n\n", begin_offset, "R*", d.reminder);

	mathmult_shift_result(d.result, 0);
	mathmult_shift_result(d.reminder, 0);
	fprintf(stream, "Z_d = %s\n", d.result);
	fprintf(stream, "R_d = %s\n", d.reminder);

	free(yminus);
}

struct result divnr(FILE* out_stream, struct mathdata data)
{
	struct mathdata_parsed parsed;
	struct result r = mathdata_parse(data, &parsed, 2);
	if (r.is_error)
		return r;


	fprintf(out_stream, "Dzielenie, metoda nierestytucyjna:\n");
	fprintf(out_stream, "X  = %c%s,%s\t", parsed.num1.is_negative ? '-' : '+', parsed.num1.whole, parsed.num1.decimal);
	fprintf(out_stream, "Y  = %c%s,%s\n\n", parsed.num2.is_negative ? '-' : '+', parsed.num2.whole, parsed.num2.decimal);

	if (parsed.base != 2)
	{
		parsed.num1 = str_convert(parsed.num1, parsed.base, 2);
		parsed.num2 = str_convert(parsed.num2, parsed.base, 2);
	}

	struct multdata d;
	mathdiv_prepdata(parsed.num1, parsed.num2, &d, parsed.precision);

	fprintf(out_stream, "X_d  = %s\n", d.num1);
	fprintf(out_stream, "Y_d  = %s\n", d.num2);
	printdiv(out_stream, d, parsed.precision);

	struct ascnumber result_num, reminder_num;
	if (d.result[0] == '1')
		str_compliment_Pm1(d.result, d.result, 2), d.result[0] = '1';
	if (d.reminder[0] == '1')
		str_compliment_Pm1(d.reminder, d.reminder, 2), d.reminder[0] = '1';

	str_to_ascnumber(d.result, &result_num);
	str_to_ascnumber(d.reminder, &reminder_num);
	result_num = str_convert(result_num, 2, parsed.base);
	reminder_num = str_convert(reminder_num, 2, parsed.base);

	fprintf(out_stream, "Z = %c%s,%s\n", d.result[0] == '0' ? '+' : '-', result_num.whole, result_num.decimal);
	fprintf(out_stream, "R = %c%s,%s\n\n", d.reminder[0] == '0' ? '+' : '-', reminder_num.whole, reminder_num.decimal);

	cleannums(&d);
	struct result res = { .is_error = false };
	return res;
}