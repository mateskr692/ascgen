#pragma once
#include <stdlib.h>
#include "multbezp.h"
#include "stdextension.h"

static void cleannums(struct multdata* d)
{
	free(d->num1);
	free(d->num2);
	free(d->result);
	
	for (int i = 0; i < d->numlen; i++)
		free(d->partial_sums[i]);
	free(d->partial_sums);

	free(d->correctS);
	free(d->correctX);
	free(d->correctY);
}

static void reserve_partials(struct multdata* d)
{
	d->correctS = malloc_s((size_t)d->numlen * 2 + 5);
	d->correctX = malloc_s((size_t)d->numlen * 2 + 5);
	d->correctY = malloc_s((size_t)d->numlen * 2 + 5);
	d->partial_sums = malloc_s(sizeof(char*) * d->numlen);
	for (int i = 0; i < d->numlen; i++)
	{
		d->partial_sums[i] = malloc_s((size_t)2 * d->numlen + 5);
		d->partial_sums[i][0] = '\0';
	}
}

static void add_partials(struct multdata d)
{
	int len = strlen_i(d.partial_sums[0]);
	if (len == 0)
	{
		d.result[0] = '0', d.result[1] = '\0';
		return;
	}

	d.result[len] = '\0';

	int carry = 0;
	for (int i = len - 1; i >= 0; i--)
	{
		int sum = carry;
		for (int j = 0; j < len - i && j < d.numlen; j++)
			sum += char_to_ascvalue(d.partial_sums[j][i]);

		d.result[i] = ascvalue_to_char(sum % 2);
		carry = sum / 2;
	}
}

static void add_corrections(struct multdata d)
{
	int len = strlen_i(d.result), carry = 0;
	for (int i = len - 1; i >= 0; i--)
	{
		if (d.result[i] == ',')
			continue;

		int sum = carry;
		sum += str_value_at(d.result, i);
		sum += str_value_at(d.correctS, i);
		sum += str_value_at(d.correctX, i);
		sum += str_value_at(d.correctY, i);

		d.result[i] = ascvalue_to_char(sum % 2);
		carry = sum / 2;
	}
}

static void printadd(FILE* stream, struct multdata d)
{
	mathmult_shift_left(d.num1, d.shift1);
	mathmult_shift_left(d.num2, d.shift2);

	fprintf(stream, "X*   = X_d * 2^(%d) = %s\n", -d.shift1, d.num1);
	fprintf(stream, "Y*   = Y_d * 2^(%d) = %s\n\n", -d.shift2, d.num2);

	int begin_offset = 7, line_width = 2 * d.numlen + 2;
	fprintf(stream, "X*   = %-*s  x0 = %c,  |X'| = %s\n", line_width, d.num1, d.num1[0], &(d.num1[2]));
	fprintf(stream, "Y*   = %-*s  y0 = %c,  |Y'| = %s\n", line_width, d.num2, d.num2[0], &(d.num2[2]));
	example_printline(stream, begin_offset, line_width);

	int plus_row = 1;
	for (; plus_row <= d.numlen; plus_row++)
	{
		if (d.num2[1 + plus_row] == '1')
			break;
	}

	for (int i = 0; i < d.numlen; i++)
	{
		int y_i = d.numlen - i;
		char y_c = d.num2[1 + y_i];
		char* code = y_c == '0' ? "" : &(d.num1[2]);

		sprintf(d.partial_sums[i], "%*s%*s", y_i, "", d.numlen, code);
		str_pad_space(d.partial_sums[i], '0');
		if (str_is_zero(d.partial_sums[i]))
			continue;

		if (y_i == plus_row)
			fprintf(stream, "%*s+ ", begin_offset, "");
		else
			fprintf(stream, "%*s", begin_offset + 2, "");

		fprintf(stream, "%-*sy%d * |X*| * 2^(-%d)\n", line_width, d.partial_sums[i], y_i, y_i);
	}

	if (!str_is_zero(d.num2) && !str_is_zero(d.num1))
		example_printline(stream, begin_offset, line_width);

	add_partials(d);
	fprintf(stream, "%*s%s  |X'| * |Y'|\n", begin_offset + 2, "",  d.result);
	
	int signX = char_to_ascvalue(d.num1[0]);
	int signY = char_to_ascvalue(d.num2[0]);
	sprintf(d.correctS, "%d,%*s", signX*signY, (unsigned int)strlen_i(d.partial_sums[0]), "");
	str_pad_space(d.correctS, '0');
	fprintf(stream, "%*s%-*s  -x0 * y0 = %d\n", begin_offset, "", line_width, d.correctS, -signX * signY);

	if (signY == 0)
	{
		sprintf(d.correctX, "0,%*s", d.numlen, "");
		str_pad_space(d.correctX, '0');
	}
	else
	{
		str_compliment_Pm1(d.num1, d.correctX, 2);
	}
	if (signX == 0)
	{
		sprintf(d.correctY, "0,%*s", d.numlen, "");
		str_pad_space(d.correctY, '0');
	}
	else
	{
		str_compliment_Pm1(d.num2, d.correctY, 2);
	}

	fprintf(stream, "%*s%-*s  -x0 * Y* = %s\n", begin_offset, "", line_width, d.correctY, signX == 0 ? "0" : "-Y*");
	fprintf(stream, "%*s+ %-*s  -y0 * X* = %s\n", begin_offset - 2, "", line_width, d.correctX, signY == 0 ? "0" : "-X*");
	example_printline(stream, begin_offset - 2, line_width + 2);

	strcat_b(d.result, "0,");
	add_corrections(d);
	fprintf(stream, "Z*   = %s\n\n", d.result);

	int shift = d.shift1 + d.shift2;
	mathmult_shift_result(d.result, shift);
	fprintf(stream, "Z_d  = %s = Z* * 2^%d\n", d.result, shift);
}


struct result multbgn(FILE* out_stream, struct mathdata data)
{
	struct mathdata_parsed parsed;
	struct result r = mathdata_parse(data, &parsed, 2);
	if (r.is_error)
		return r;

	char* n1sign = parsed.num1.used_sign_bit ?
		parsed.num1.is_negative ? "1." : "0." :
		parsed.num1.is_negative ? "-" : "+";
	char* n2sign = parsed.num2.used_sign_bit ?
		parsed.num2.is_negative ? "1." : "0." :
		parsed.num2.is_negative ? "-" : "+";

	fprintf(out_stream, "Mnozenie, metoda BGN:\n");
	fprintf(out_stream, "X = %s%s,%s\t", n1sign, parsed.num1.whole, parsed.num1.decimal);
	fprintf(out_stream, "Y = %s%s,%s\n\n", n2sign, parsed.num2.whole, parsed.num2.decimal);

	if (parsed.base != 2)
	{
		parsed.num1 = str_convert(parsed.num1, parsed.base, 2);
		parsed.num2 = str_convert(parsed.num2, parsed.base, 2);
	}

	struct multdata d;
	mathmult_prepdata(parsed.num1, parsed.num2, &d);
	reserve_partials(&d);

	fprintf(out_stream, "X_d  = %s\n", d.num1);
	fprintf(out_stream, "Y_d  = %s\n", d.num2);
	printadd(out_stream, d);


	struct ascnumber result_num;
	if (d.result[0] == '1')
	{
		str_compliment_Pm1(d.result, d.result, 2);
		d.result[0] = '1';
	}

	str_to_ascnumber(d.result, &result_num);
	result_num = str_convert(result_num, 2, parsed.base);
	fprintf(out_stream, "Z    =  %c%s,%s\n\n", d.result[0] == '0' ? '+' : '-', result_num.whole, result_num.decimal);

	cleannums(&d);
	struct result res = { .is_error = false };
	return res;
}