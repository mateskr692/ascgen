#pragma once
#include <stdlib.h>
#include "add8421.h"
#include "stdextension.h"

static struct tetrade map_raw[] = 
{
	[0]  = {"0000", 0},
	[1]  = {"0001", 0},
	[2]  = {"0010", 0},
	[3]  = {"0011", 0},
	[4]  = {"0100", 0},
	[5]  = {"0101", 0},
	[6]  = {"0110", 0},
	[7]  = {"0111", 0},
	[8]  = {"1000", 0},
	[9]  = {"1001", 0},
	//----------------
	[10] = {"1010", 0},
	[11] = {"1011", 0},
	[12] = {"1100", 0},
	[13] = {"1101", 0},
	[14] = {"1110", 0},
	[15] = {"1111", 0},
	[16] = {"0000", 1},
	[17] = {"0001", 1},
	[18] = {"0010", 1},
	[19] = {"0011", 1},
};

static struct tetrade map_correct[] =
{
	[0]  = {"0000", 0},
	[1]  = {"0001", 0},
	[2]  = {"0010", 0},
	[3]  = {"0011", 0},
	[4]  = {"0100", 0},
	[5]  = {"0101", 0},
	[6]  = {"0110", 0},
	[7]  = {"0111", 0},
	[8]  = {"1000", 0},
	[9]  = {"1001", 0},
	//----------------
	[10] = {"0000", 1},
	[11] = {"0001", 1},
	[12] = {"0010", 1},
	[13] = {"0011", 1},
	[14] = {"0100", 1},
	[15] = {"0101", 1},
	[16] = {"0110", 1},
	[17] = {"0111", 1},
	[18] = {"1000", 1},
	[19] = {"1001", 1},
};

static char* correction(int value)
{
	return value <= 9 ? NULL : "0110";
}

static int print_number(FILE* stream, char* num)
{
	int i = 0, s = 0, len = strlen_i(num);
	if (len > 2 && num[1] == '.')
	{
		s += fprintf(stream, "%c.", num[0]);
		i = 2;
	}

	for (; i < len; i++)
	{
		if (num[i] == ',')
			continue;

		int val = char_to_ascvalue(num[i]);
		char* code = val >= 10 ? map_raw[val].code : map_correct[val].code;
		if (num[i + 1] == '\0')
			s += fprintf(stream, "%s", code);
		else
			s += fprintf(stream, "%s%c", code, num[i + 1] == ',' ? ',' : ' ');
	}
	return s;
}

static bool predict_overflow(char* num1, char* num2)
{
	int carry = 0;
	for (int i = strlen_i(num1) - 1; i >= 0; i--)
	{
		if (num1[i] == ',')
			continue;
		carry = (char_to_ascvalue(num1[i]) + char_to_ascvalue(num2[i]) + carry) / 10;
	}
	return carry > 0;
}

static int printapply_correction(FILE* stream, char* result, int line_width, int begin_offset, int number_offset)
{
	int len = strlen_i(result), tetrade_offset = 0, i = 0, carry = 0;
	bool needs_correction = false;
	if (len > 2 && result[1] == '.')
		tetrade_offset = 2;

	while (true)
	{
		for (i = len - 1; i >= tetrade_offset; i--)
		{
			if (result[i] == ',')
				continue;
			char* corr = correction(char_to_ascvalue(result[i]));
			if (corr == NULL)
				continue;

			needs_correction = true;
			break;
		}
		if (!needs_correction)
			return carry;

		fprintf(stream, "%*s", begin_offset + number_offset, "");
		print_number(stream, result);
		fprintf(stream, "\n%*s", begin_offset + number_offset + tetrade_offset, "");

		for (int i = tetrade_offset; i < len; i++)
		{
			if (result[i] == ',')
				continue;
			char* corr = correction(char_to_ascvalue(result[i]));
			fprintf(stream, "%-5s", corr == NULL ? "" : corr);

			if (corr != NULL)
			{
				int val = char_to_ascvalue(result[i]);
				result[i] = ascvalue_to_char(val - 10);
				if (map_correct[val].carry == 1 && map_raw[val].carry == 0)
				{
					if (i == 0)
					{
						carry = 1;
						continue;
					}
					if (i == 2 && result[1] == '.')
					{
						int val = char_to_ascvalue(result[0]) + 1;
						result[0] = ascvalue_to_char(val % 2);
						carry = val / 2;
						continue;
					}

					int prev = result[i - 1] == ',' ? i - 2 : i - 1;
					result[prev] = ascvalue_to_char(char_to_ascvalue(result[prev]) + 1);
				}
			}
		}
		fprintf(stream, "+ poprawka\n");
		example_printline(stream, begin_offset, line_width);
		needs_correction = false;
	}
}


static void printadd_mod(FILE* stream, char* num1, char sign1, char* num2, char sign2, char* result, char* sign_r)
{
	fprintf(stream, "X_m = %c.", sign1);
	print_number(stream, num1);
	fprintf(stream, "\tx_s = %c\n", sign1);

	fprintf(stream, "Y_m = %c.", sign2);
	print_number(stream, num2);
	fprintf(stream, "\tx_s = %c\n", sign2);

	//add modules, sign remains the same
	if (sign1 == sign2)
	{
		int row_width = 0, begin_offset = 8;
		fprintf(stream, "x_s = y_s = z_s = %c\n\n", sign1);

		bool ov = predict_overflow(num1, num2);
		int number_offset = ov ? 5 : 0;

		row_width += fprintf(stream, "|X| =   %*s", number_offset, "");
		row_width += print_number(stream, num1);
		fprintf(stream, "\n|Y| =   %*s", number_offset, "");
		print_number(stream, num2);
		fprintf(stream, " +\n");

		int line_width = row_width - begin_offset;
		example_printline(stream, begin_offset, line_width);

		int carry = str_addnum_bcd(num1, num2, result, map_raw);
		if (carry == 1)
			strcat_b(result, "1"), number_offset = 0;
		carry = printapply_correction(stream, result, line_width, begin_offset, number_offset);
		if (carry == 1)
			strcat_b(result, "1"), number_offset = 0;

		fprintf(stream, "|Z| =   ");
		print_number(stream, result);
		fprintf(stream, "\n\nZ_m = %c.", *sign_r);
		print_number(stream, result);
		fprintf(stream, " = x_s.|Z|\n");
	}
	else
	{
		//add num1 module with ones compliment of num2 module
		str_compliment_P(num2, num2, 10);
		fprintf(stream, "x_s != y_s\n\n");
		int row_width = 0, begin_offset = 8;

		row_width += fprintf(stream, "|X|  =  ");
		row_width += print_number(stream, num1);
		fprintf(stream, "\n|Y|' =  ");
		print_number(stream, num2);
		fprintf(stream, " +\n");

		int line_width = row_width - begin_offset;
		example_printline(stream, begin_offset, line_width);

		int carry = str_addnum_bcd(num1, num2, result, map_raw);
		carry += printapply_correction(stream, result, line_width, begin_offset, 0);
		fprintf(stream, "   %d <- ", carry);
		print_number(stream, result);
		fprintf(stream, "\n");

		//if carry occurs, add it, and use num1 sign as result sign
		if (carry > 0)
		{
			int carry_offset = 3;
			int carry_width = line_width + (begin_offset - carry_offset);
			example_printcarry(stream, carry_offset, carry_width);
			example_printline(stream, begin_offset, line_width);
			str_addconst_bcd(result, result, map_raw, 1);
			printapply_correction(stream, result, line_width, begin_offset, 0);

			fprintf(stream, "|Z|  =  ");
			print_number(stream, result);
			fprintf(stream, "\n\nZ_m = %c.", *sign_r);
			print_number(stream, result);
			fprintf(stream, " = x_s.|Z|\n");

		}
		//if no carry occurs, ones compliment the result and use num2 sign as result sign
		else
		{
			*sign_r = sign2;
			str_compliment_P(result, num2, 10);

			fprintf(stream, "|Z| =  (");
			print_number(stream, result);
			fprintf(stream, ")' = ");
			print_number(stream, num2);
			strcpy(result, num2);
			fprintf(stream, "\n\nZ_m = %c.", *sign_r);
			print_number(stream, result);
			fprintf(stream, " = y_s.|Z|\n");
		}
	}
}

static void printadd_odw(FILE* stream, char* num1, char* num2, char* result)
{
	int row_width = 0, begin_offset = 6;
	
	row_width += fprintf(stream, "X_o = ");
	row_width += print_number(stream, num1);
	fprintf(stream, "\nY_o = ");
	print_number(stream, num2);
	fprintf(stream, " +\n");

	int line_width = row_width - begin_offset;
	example_printline(stream, begin_offset, line_width);

	int carry = str_addnum_bcd(num1, num2, result, map_raw);
	carry += printapply_correction(stream, result, line_width, begin_offset, 0);
	if (carry == 0)
	{
		fprintf(stream, "Z_o = ");
		print_number(stream, result);
		fprintf(stream, "\n\n");
	}
	else
	{
		int carry_offset = 1;
		int carry_width = line_width + (begin_offset - carry_offset);

		fprintf(stream, " 1 <- ");
		print_number(stream, result);
		fprintf(stream, "\n");
		example_printcarry(stream, carry_offset, carry_width);
		example_printline(stream, begin_offset, line_width);

		str_addconst_bcd(result, result, map_raw, 1);
		printapply_correction(stream, result, line_width, begin_offset, 0);

		fprintf(stream, "Z_o = ");
		print_number(stream, result);
		fprintf(stream, "\n\n");
	}
}

static void printadd_dop(FILE* stream, char* num1, char* num2, char* result)
{
	int row_width = 0, begin_offset = 6;

	row_width += fprintf(stream, "X_d = ");
	row_width += print_number(stream, num1);
	fprintf(stream, "\nY_d = ");
	print_number(stream, num2);
	fprintf(stream, " +\n");

	int line_width = row_width - begin_offset;
	example_printline(stream, begin_offset, line_width);

	str_addnum_bcd(num1, num2, result, map_raw);
	printapply_correction(stream, result, line_width, begin_offset, 0);
	fprintf(stream, "Z_d = ");
	print_number(stream, result);
	fprintf(stream, "\n\n");
}


struct result add8421_mod(FILE* out_stream, struct mathdata data)
{
	//parse data
	struct mathdata_parsed parsed;
	struct result r = mathdata_parse(data, &parsed, 10);
	if (r.is_error)
		return r;

	char* n1sign = parsed.num1.used_sign_bit ?
		parsed.num1.is_negative ? "1." : "0." :
		parsed.num1.is_negative ? "-" : "+";
	char* n2sign = parsed.num2.used_sign_bit ?
		parsed.num2.is_negative ? "1." : "0." :
		parsed.num2.is_negative ? "-" : "+";

	fprintf(out_stream, "Dodawanie BCD w kodzie 8421, zapis modulowy:\n");
	fprintf(out_stream, "X = %s%s,%s\t", n1sign, parsed.num1.whole, parsed.num1.decimal);
	fprintf(out_stream, "Y = %s%s,%s\n\n", n2sign, parsed.num2.whole, parsed.num2.decimal);

	char sign1 = parsed.num1.is_negative ? '1' : '0';
	char sign2 = parsed.num2.is_negative ? '1' : '0';
	char sign_r = sign1;
	char* num1 = NULL, * num2 = NULL, * result = NULL;
	if (parsed.base != 10)
	{
		parsed.num1 = str_convert(parsed.num1, parsed.base, 10);
		parsed.num2 = str_convert(parsed.num2, parsed.base, 10);
	}
	mathadd_prepdata_mod(parsed.num1, parsed.num2, 10, &num1, &num2, &result);
	printadd_mod(out_stream, num1, sign1, num2, sign2, result, &sign_r);

	struct ascnumber result_num;
	str_to_ascnumber(result, &result_num);
	result_num = str_convert(result_num, 10, parsed.base);
	fprintf(out_stream, "Z   =  %c%s,%s\n\n", sign_r == '0' ? '+' : '-', result_num.whole, result_num.decimal);

	free(num1), free(num2), free(result);
	struct result res = { .is_error = false };
	return res;
}

struct result add8421_odw(FILE* out_stream, struct mathdata data)
{
	//parse data
	struct mathdata_parsed parsed;
	struct result r = mathdata_parse(data, &parsed, 10);
	if (r.is_error)
		return r;

	char* n1sign = parsed.num1.used_sign_bit ?
		parsed.num1.is_negative ? "1." : "0." :
		parsed.num1.is_negative ? "-" : "+";
	char* n2sign = parsed.num2.used_sign_bit ?
		parsed.num2.is_negative ? "1." : "0." :
		parsed.num2.is_negative ? "-" : "+";

	fprintf(out_stream, "Dodawanie BCD w kodzie 8421, zapis odwrotny:\n");
	fprintf(out_stream, "X = %s%s,%s\t", n1sign, parsed.num1.whole, parsed.num1.decimal);
	fprintf(out_stream, "Y = %s%s,%s\n\n", n2sign, parsed.num2.whole, parsed.num2.decimal);

	char* num1 = NULL, * num2 = NULL, * result = NULL;
	if (parsed.base != 10)
	{
		parsed.num1 = str_convert(parsed.num1, parsed.base, 10);
		parsed.num2 = str_convert(parsed.num2, parsed.base, 10);
	}
	mathadd_prepdata(parsed.num1, parsed.num2, 10, &num1, &num2, &result, false);

	//add numbers
	printadd_odw(out_stream, num1, num2, result);

	//detect overflow
	if (num1[0] == num2[0] && result[0] != num1[0])
	{
		fprintf(out_stream, "x_s = y_s != z_s -> nadmiar\n\n");
		if (num1[0] == '0')
			strcat_b(&num1[2], "0");
		else
			strcat_b(&num1[2], "9");

		if (num2[0] == '0')
			strcat_b(&num2[2], "0");
		else
			strcat_b(&num2[2], "9");

		//add expanded
		printadd_odw(out_stream, num1, num2, result);
	}

	struct ascnumber result_num;
	if (result[0] == '1')
	{
		str_compliment_P(result, result, 10);
		result[0] = '1';
	}

	str_to_ascnumber(result, &result_num);
	result_num = str_convert(result_num, 10, parsed.base);
	fprintf(out_stream, "Z   =  %c%s,%s\n\n", result[0] == '0' ? '+' : '-', result_num.whole, result_num.decimal);

	free(num1), free(num2), free(result);
	struct result res = { .is_error = false };
	return res;
}

struct result add8421_dop(FILE* out_stream, struct mathdata data)
{
	//parse data
	struct mathdata_parsed parsed;
	struct result r = mathdata_parse(data, &parsed, 10);
	if (r.is_error)
		return r;

	char* n1sign = parsed.num1.used_sign_bit ?
		parsed.num1.is_negative ? "1." : "0." :
		parsed.num1.is_negative ? "-" : "+";
	char* n2sign = parsed.num2.used_sign_bit ?
		parsed.num2.is_negative ? "1." : "0." :
		parsed.num2.is_negative ? "-" : "+";

	fprintf(out_stream, "Dodawanie BCD w kodzie 8421, zapis dopelnieniowy:\n");
	fprintf(out_stream, "X = %s%s,%s\t", n1sign, parsed.num1.whole, parsed.num1.decimal);
	fprintf(out_stream, "Y = %s%s,%s\n\n", n2sign, parsed.num2.whole, parsed.num2.decimal);

	char* num1 = NULL, * num2 = NULL, * result = NULL;
	if (parsed.base != 10)
	{
		parsed.num1 = str_convert(parsed.num1, parsed.base, 10);
		parsed.num2 = str_convert(parsed.num2, parsed.base, 10);
	}
	mathadd_prepdata(parsed.num1, parsed.num2, 10, &num1, &num2, &result, true);

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
			str_compliment_Pm1(num1, num1, 10);
			strcat_b(&num1[2], "0");
			str_compliment_Pm1(num1, num1, 10);
		}

		if (num2[0] == '0')
			strcat_b(&num2[2], "0");
		else
		{
			str_compliment_Pm1(num2, num2, 10);
			strcat_b(&num2[2], "0");
			str_compliment_Pm1(num2, num2, 10);
		}

		//add expanded
		printadd_dop(out_stream, num1, num2, result);
	}

	struct ascnumber result_num;
	if (result[0] == '1')
	{
		str_compliment_Pm1(result, result, 10);
		result[0] = '1';
	}

	str_to_ascnumber(result, &result_num);
	result_num = str_convert(result_num, 10, parsed.base);
	fprintf(out_stream, "Z   =  %c%s,%s\n\n", result[0] == '0' ? '+' : '-', result_num.whole, result_num.decimal);

	free(num1), free(num2), free(result);
	struct result res = { .is_error = false };
	return res;
}
