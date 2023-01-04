#pragma once
#include <stdlib.h>
#include "multbezp.h"
#include "stdextension.h"

static void reserve_partials(struct multdata* d)
{
	d->partial_sums = malloc_s(sizeof(char*) * d->numlen);
	for (int i = 0; i < d->numlen; i++)
	{
		d->partial_sums[i] = malloc_s((size_t)2 * d->numlen + 5);
		d->partial_sums[i][0] = '\0';
	}
}

static void cleannums(struct multdata* d)
{
	free(d->num1);
	free(d->num2);
	free(d->result);
	for (int i = 0; i < d->numlen; i++)
		free(d->partial_sums[i]);
	free(d->partial_sums);
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


static void printadd(FILE* stream, struct multdata d)
{
	mathmult_shift_left(d.num1, d.shift1);
	mathmult_shift_left(d.num2, d.shift2);

	fprintf(stream, "X*   = X_m * 2^(%d) = %s\n", -d.shift1, d.num1);
	fprintf(stream, "Y*   = Y_m * 2^(%d) = %s\n\n", -d.shift2, d.num2);

	int begin_offset = 7, line_width = 2 * d.numlen + 2;
	fprintf(stream, "X*   = %-*s  x0 = %c,  |X*| = %s\n", line_width, d.num1, d.num1[0], &(d.num1[2]));
	fprintf(stream, "Y*   = %-*s  y0 = %c,  |Y*| = %s\n", line_width, d.num2, d.num2[0], &(d.num2[2]));
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
	int sign = char_to_ascvalue(d.num1[0]) ^ char_to_ascvalue(d.num2[0]);
	int shift = d.shift1 + d.shift2;
	fprintf(stream, "|Z*| =   %s\n", d.result);
	
	strcat_b(d.result, sign == 0 ? "0," : "1,");
	fprintf(stream, " Z*  = %s  z0 = (x0 xor y0) = %d\n\n", d.result, sign);

	mathmult_shift_result(d.result, shift);
	fprintf(stream, "Z_m  = %s = Z* * 2^%d\n", d.result, shift);

}

struct result multbezp(FILE* out_stream, struct mathdata data)
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

	fprintf(out_stream, "Mnozenie, metoda bezposrednia:\n");
	fprintf(out_stream, "X = %s%s,%s\t", n1sign, parsed.num1.whole, parsed.num1.decimal);
	fprintf(out_stream, "Y = %s%s,%s\n\n", n2sign, parsed.num2.whole, parsed.num2.decimal);

	if (parsed.base != 2)
	{
		parsed.num1 = str_convert(parsed.num1, parsed.base, 2);
		parsed.num2 = str_convert(parsed.num2, parsed.base, 2);
	}

	fprintf(out_stream, "X_m  = %c.%s,%s\n", parsed.num1.is_negative ? '1' : '0', parsed.num1.whole, parsed.num1.decimal);
	fprintf(out_stream, "Y_m  = %c.%s,%s\n", parsed.num2.is_negative ? '1' : '0', parsed.num2.whole, parsed.num2.decimal);

	struct multdata d;
	mathmult_prepdata_mod(parsed.num1, parsed.num2, &d);
	reserve_partials(&d);
	printadd(out_stream, d);
	
	struct ascnumber result_num;
	str_to_ascnumber(d.result, &result_num);
	result_num = str_convert(result_num, 2, parsed.base);
	fprintf(out_stream, "Z    =  %c%s,%s\n\n", d.result[0] == '0' ? '+' : '-', result_num.whole, result_num.decimal);

	cleannums(&d);
	struct result res = { .is_error = false };
	return res;
	
}