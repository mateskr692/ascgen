#pragma once
#include <stdlib.h>
#include "addbin.h"
#include "stdextension.h"

static void printadd_mod(FILE* stream, char* num1, char sign1, char* num2, char sign2, char* result, char* sign_r)
{
	fprintf(stream, "X_m = %c.%s\tx_s = %c\n", sign1, num1, sign1);
	fprintf(stream, "Y_m = %c.%s\ty_s = %c\n", sign2, num2, sign2);
	int base = 2;

	//add modules, sign remains the same
	if (sign1 == sign2)
	{
		int row_width = 0, begin_offset = 8, number_offset = 0;
		int carry = str_addnum(num1, num2, result, base);
		if (carry == 1)
		{
			strcat_b(result, "1");
			number_offset = 1;
		}

		fprintf(stream, "x_s = y_s = z_s = %c\n\n", sign1);
		row_width += fprintf(stream, "|X| =   %*s%s\n", number_offset, "", num1) - 1;
		fprintf(stream, "|Y| =   %*s%s +\n", number_offset, "", num2);
		int line_width = row_width - begin_offset;

		example_printline(stream, begin_offset, line_width);
		fprintf(stream, "|Z| =   %s\n\n", result);
		fprintf(stream, "Z_m = %c.%s = x_s.|Z|\n", *sign_r, result);
	}
	else
	{
		//add num1 module with ones compliment of num2 module
		str_compliment_P(num2, num2, 2);
		fprintf(stream, "x_s != y_s\n\n");
		int row_width = 0, begin_offset = 8;

		row_width += fprintf(stream, "|X|  =  %s\n", num1) - 1;
		fprintf(stream, "|Y|' =  %s +\n", num2);
		int line_width = row_width - begin_offset;
		example_printline(stream, begin_offset, line_width);

		int carry = str_addnum(num1, num2, result, base);
		fprintf(stream, "   %d <- %s\n", carry, result);

		//if carry occurs, add it, and use num1 sign as result sign
		if (carry > 0)
		{
			int carry_offset = 3;
			int carry_width = line_width + (begin_offset - carry_offset);
			example_printcarry(stream, carry_offset, carry_width);
			example_printline(stream, begin_offset, line_width);

			str_addconst(result, result, base, 1);
			fprintf(stream, "|Z|  =  %s\n\n", result);
			fprintf(stream, "Z_m = %c.%s = x_s.|Z|\n", *sign_r, result);
		}
		//if no carry occurs, ones compliment the result and use num2 sign as result sign
		else
		{
			*sign_r = sign2;
			str_compliment_P(result, num2, 2);
			fprintf(stream, "|Z|  = (%s)' = %s\n\n", result, num2);
			strcpy(result, num2);
			fprintf(stream, "Z_m = %c.%s = y_s.|Z|\n", *sign_r, result);
		}
	}
}

static void printadd_odw(FILE* stream, char* num1, char* num2, char* result)
{
	int base = 2;
	int row_width = 0, begin_offset = 6;
	row_width += fprintf(stream, "X_o = %s\n", num1) - 1;
	fprintf(stream, "Y_o = %s +\n", num2);
	int line_width = row_width - begin_offset;
	example_printline(stream, begin_offset, line_width);

	int carry = str_addnum(num1, num2, result, base);
	if (carry == 0)
		fprintf(stream, "Z_o = %s\n\n", result);
	else
	{
		int carry_offset = 1;
		int carry_width = line_width + (begin_offset - carry_offset);

		fprintf(stream, " 1 <- %s\n", result);
		example_printcarry(stream, carry_offset, carry_width);
		example_printline(stream, begin_offset, line_width);

		str_addconst(result, result, base, 1);
		fprintf(stream, "Z_o = %s\n\n", result);
	}
}

static void printadd_dop(FILE* stream, char* num1, char* num2, char* result)
{
	int base = 2;
	int row_width = 0, begin_offset = 6;
	row_width += fprintf(stream, "X_d = %s\n", num1) - 1;
	fprintf(stream, "Y_d = %s +\n", num2);
	int line_width = row_width - begin_offset;
	example_printline(stream, begin_offset, line_width);

	str_addnum(num1, num2, result, base);
	fprintf(stream, "Z_d = %s\n\n", result);
}

struct result addbin_mod(FILE* out_stream, struct mathdata data)
{
	//parse data
	struct mathdata_parsed parsed;
	struct result r = mathdata_parse(data, &parsed, 2);
	if (r.is_error)
		return r;
	
	char* n1sign = parsed.num1.used_sign_bit ?
		parsed.num1.is_negative ? "1." : "0." :
		parsed.num1.is_negative ? "-"  : "+";
	char* n2sign = parsed.num2.used_sign_bit ?
		parsed.num2.is_negative ? "1." : "0." :
		parsed.num2.is_negative ? "-"  : "+";

	fprintf(out_stream, "Dodawanie dwojkowe, zapis modulowy:\n");
	fprintf(out_stream, "X = %s%s,%s\t", n1sign, parsed.num1.whole, parsed.num1.decimal);
	fprintf(out_stream, "Y = %s%s,%s\n\n", n2sign, parsed.num2.whole, parsed.num2.decimal);
	
	char sign1 = parsed.num1.is_negative ? '1' : '0';
	char sign2 = parsed.num2.is_negative ? '1' : '0';
	char sign_r = sign1;
	char *num1 = NULL, *num2 = NULL, *result = NULL;

	if (parsed.base != 2)
	{
		parsed.num1 = str_convert(parsed.num1, parsed.base, 2);
		parsed.num2 = str_convert(parsed.num2, parsed.base, 2);
	}
	mathadd_prepdata_mod(parsed.num1, parsed.num2, 2, &num1, &num2, &result);
	printadd_mod(out_stream, num1, sign1, num2, sign2, result, &sign_r);

	struct ascnumber result_num;
	str_to_ascnumber(result, &result_num);
	result_num = str_convert(result_num, 2, parsed.base);
	fprintf(out_stream, "Z   =  %c%s,%s\n\n", sign_r == '0' ? '+' : '-', result_num.whole, result_num.decimal);
	
	free(num1), free(num2), free(result);
	struct result res = { .is_error = false };
	return res;
}

struct result addbin_odw(FILE* out_stream, struct mathdata data)
{
	//parse data
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

	fprintf(out_stream, "Dodawanie dwojkowe, zapis odwrotny:\n");
	fprintf(out_stream, "X = %s%s,%s\t", n1sign, parsed.num1.whole, parsed.num1.decimal);
	fprintf(out_stream, "Y = %s%s,%s\n\n", n2sign, parsed.num2.whole, parsed.num2.decimal);

	char* num1 = NULL, * num2 = NULL, * result = NULL;
	if (parsed.base != 2)
	{
		parsed.num1 = str_convert(parsed.num1, parsed.base, 2);
		parsed.num2 = str_convert(parsed.num2, parsed.base, 2);
	}
	mathadd_prepdata(parsed.num1, parsed.num2, 2, &num1, &num2, &result, false);

	//add numbers
	printadd_odw(out_stream, num1, num2, result);

	//detect overflow
	if (num1[0] == num2[0] && result[0] != num1[0])
	{
		fprintf(out_stream, "x_s = y_s != z_s -> nadmiar\n\n");
		if(num1[0] == '0')
			strcat_b(&num1[2], "0");
		else
			strcat_b(&num1[2], "1");

		if(num2[0] == '0')
			strcat_b(&num2[2], "0");
		else
			strcat_b(&num2[2], "1");

		//add expanded
		printadd_odw(out_stream, num1, num2, result);
	}

	struct ascnumber result_num;
	if (result[0] == '1')
	{
		str_compliment_P(result, result, 2);
		result[0] = '1';
	}

	str_to_ascnumber(result, &result_num);
	result_num = str_convert(result_num, 2, parsed.base);
	fprintf(out_stream, "Z   =  %c%s,%s\n\n", result[0] == '0' ? '+' : '-', result_num.whole, result_num.decimal);

	free(num1), free(num2), free(result);
	struct result res = { .is_error = false };
	return res;
}

struct result addbin_dop(FILE* out_stream, struct mathdata data)
{
	//parse data
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

	fprintf(out_stream, "Dodawanie dwojkowe, zapis dopelnieniowy:\n");
	fprintf(out_stream, "X = %s%s,%s\t", n1sign, parsed.num1.whole, parsed.num1.decimal);
	fprintf(out_stream, "Y = %s%s,%s\n\n", n2sign, parsed.num2.whole, parsed.num2.decimal);

	char* num1 = NULL, * num2 = NULL, * result = NULL;
	if (parsed.base != 2)
	{
		parsed.num1 = str_convert(parsed.num1, parsed.base, 2);
		parsed.num2 = str_convert(parsed.num2, parsed.base, 2);
	}
	mathadd_prepdata(parsed.num1, parsed.num2, 2, &num1, &num2, &result, true);

	//add numbers
	printadd_dop(out_stream, num1, num2, result);

	//detect overflow
	if (num1[0] == num2[0] && result[0] != num1[0])
	{
		fprintf(out_stream, "x_s = y_s != z_s -> nadmiar\n\n");
		if (num1[0] == '0')
			strcat_b(&num1[2], "0");
		else
		{
			str_compliment_Pm1(num1, num1, 2);
			strcat_b(&num1[2], "0");
			str_compliment_Pm1(num1, num1, 2);
		}
		if (num2[0] == '0')
			strcat_b(&num2[2], "0");
		else
		{
			str_compliment_Pm1(num2, num2, 2);
			strcat_b(&num2[2], "0");
			str_compliment_Pm1(num2, num2, 2);
		}

		//add expanded
		printadd_dop(out_stream, num1, num2, result);

	}

	struct ascnumber result_num;
	if (result[0] == '1')
	{
		str_compliment_Pm1(result, result, 2);
		result[0] = '1';
	}
	str_to_ascnumber(result, &result_num);
	result_num = str_convert(result_num, 2, parsed.base);
	fprintf(out_stream, "Z   =  %c%s,%s\n\n", result[0] == '0' ? '+' : '-', result_num.whole, result_num.decimal);

	free(num1), free(num2), free(result);
	struct result res = { .is_error = false };
	return res;
}