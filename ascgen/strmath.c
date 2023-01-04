#pragma once
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "strmath.h"
#include "stdextension.h"


struct result str_to_int(char* str, int* num, int base)
{
	*num = 0;
	int len;
	if (str == NULL || (len = strlen_i(str)) == 0)
		return (struct result) { .is_error = true, .message = "Given number is an empty string\n" };

	for (int i = 0; i < len; i++)
	{
		int value = char_to_ascvalue(str[i]);
		if (value == -1)
		{
			struct result res = { .is_error = true };
			sprintf(res.message, "Invalid character '%c' in number '%s'\n", str[i], str);
			return res;
		}
		if (value >= base)
		{
			struct result res = { .is_error = true };
			sprintf(res.message, "Numbers %s has digit %c but is only of base %d", str, str[i], base);
			return res;
		}

		*num += value * pow_i(base, len - 1 - i);
	}

	return (struct result) { .is_error = false };
}

void int_to_str(char* str, int num, int base)
{
	if (num == 0)
	{
		str[0] = '0', str[1] = '\0';
		return;
	}

	int i = 0;
	while (num > 0)
	{
		int reminder = num % base;
		num /= base;
		str[i] = ascvalue_to_char(reminder);
		i++;
	}
	str[i] = '\0';
	str_reverse(str);
}

int str_is_greater(char* str1, char* str2)
{
	char s1[STR_SIZE_DEF], s2[STR_SIZE_DEF];
	str_trim_leading(str1, s1, '0');
	str_trim_leading(str2, s2, '0');
	int l1 = strlen_i(s1), l2 = strlen_i(s2);
	if (l1 > l2)
		return 1;
	if (l1 < l2)
		return -1;

	int i = 0;
	while (true)
	{
		if (s1[i] == '\0')
		{
			if (s2[i] == '\0')
				return 0;
			return -1;
		}
		else if (s2[i] == '\0')
			return 1;

		int v1 = char_to_ascvalue(s1[i]), v2 = char_to_ascvalue(s2[i]);
		if (v1 > v2)
			return 1;
		if (v1 < v2)
			return -1;

		i++;
	}
}

int str_decimal_is_greater(char* s1, char* s2)
{
	int i = 0;
	while (true)
	{
		if (s1[i] == '\0')
		{
			if (s2[i] == '\0')
				return 0;
			return -1;
		}
		else if (s2[i] == '\0')
			return 1;

		int v1 = char_to_ascvalue(s1[i]), v2 = char_to_ascvalue(s2[i]);
		if (v1 > v2)
			return 1;
		if (v1 < v2)
			return -1;

		i++;
	}
}

void str_pad_space(char* str, char c)
{
	for (int i = 0; i < strlen_i(str); i++)
	{
		if (str[i] == ' ')
			str[i] = c;
	}
}

void str_trim_leading(char* str, char* res, char c)
{
	char buff[STR_SIZE_DEF];

	int offset = 0;
	while (str[offset] == c)
		offset++;

	strcpy(buff, &str[offset]);
	strcpy(res, buff);
}

double str_deciaml_value(char* str, int base)
{
	double value = 0.0;
	for (int i = 0; i < strlen_i(str); i++)
	{
		if (str[i] == '0')
			continue;

		value += (double)char_to_ascvalue(str[i]) / pow(base, i + 1);
	}

	return value;
}

void str_pow(int base, int exponent, char* result, int result_base)
{
	result[0] = '1', result[1] = '\0';
	while (exponent > 0)
	{
		str_whole_multiply(result, result, result_base, base);
		exponent--;
	}
}

void str_whole_multiply(char* str, char* result, int base, int multiplier)
{
	if (multiplier == 0)
	{
		result[0] = '0', result[1] = '\0';
		return;
	}

	char multiplied[STR_SIZE_DEF] = "";
	int reminder = 0, pos = 0, len = strlen_i(str);
	for (int i = len - 1; i >= 0; i--)
	{
		int num = char_to_ascvalue(str[i]) * multiplier + reminder;
		multiplied[pos++] = ascvalue_to_char(num % base);
		reminder = num / base;
	}
	while (reminder > 0)
	{
		multiplied[pos++] = ascvalue_to_char(reminder % base);
		reminder /= base;
	}
	multiplied[pos] = '\0';
	str_reverse(multiplied);
	strcpy(result, multiplied);
}

int str_decimal_multiply(char* str, char* result, int base, int multiplier)
{
	char multiplied[STR_SIZE_DEF] = "";
	int reminder = 0, len = strlen_i(str);
	for (int i = len - 1; i >= 0; i--)
	{
		int num = char_to_ascvalue(str[i]) * multiplier + reminder;
		multiplied[i] = ascvalue_to_char(num % base);
		reminder = num / base;
	}
	multiplied[len] = '\0';
	strcpy(result, multiplied);
	return reminder;
}

void str_whole_subtract(char* str1, char* str2, char* result, int base)
{
	int len = strlen_i(str1);
	char s2[STR_SIZE_DEF] = "", subtracted[STR_SIZE_DEF] = "";
	sprintf(s2, "%*s", len, str2);
	str_pad_space(s2, '0');

	bool borrow = false;
	for (int i = len - 1; i >= 0; i--)
	{
		int v1 = char_to_ascvalue(str1[i]), v2 = char_to_ascvalue(s2[i]);
		if (borrow)
			v1--, borrow = false;
		if (v1 < v2)
			v1 += base, borrow = true;
		subtracted[i] = ascvalue_to_char(v1 - v2);
	}
	subtracted[len] = '\0';

	if (str_is_zero(subtracted))
		result[0] = '0', result[1] = '\0';
	else
	{
		int offset = 0;
		while (subtracted[offset] == '0')
			offset++;
		strcpy(result, &subtracted[offset]);
	}
}

void str_decimal_subtract(char* str1, char* str2, char* result, int base)
{
	int len = max_i(strlen_i(str1), strlen_i(str2));
	char s1[STR_SIZE_DEF] = "", s2[STR_SIZE_DEF] = "", subtracted[STR_SIZE_DEF] = "";
	sprintf(s1, "%-*s", len, str1), sprintf(s2, "%-*s", len, str2);
	str_pad_space(s1, '0'), str_pad_space(s2, '0');

	bool borrow = false;
	for (int i = len-1; i >= 0; i--)
	{
		int v1 = char_to_ascvalue(s1[i]), v2 = char_to_ascvalue(s2[i]);
		if (borrow)
			v1--, borrow = false;
		if (v1 < v2)
			v1 += base, borrow = true;
		subtracted[i] = ascvalue_to_char(v1 - v2);
	}
	subtracted[len] = '\0';
	strcpy(result, subtracted);
}

int str_whole_divide_constant(char* str, char* result, int base, int divisor)
{
	char divided[STR_SIZE_DEF] = "";
	int reminder = 0, j = 0, len = strlen_i(str);
	for (int i = 0; i < len; i++)
	{
		reminder *= base;
		reminder += char_to_ascvalue(str[i]);
		if (reminder >= divisor)
		{
			divided[j++] = ascvalue_to_char(reminder / divisor);
			reminder %= divisor;
		}
		else if (j > 0)
		{
			divided[j++] = '0';
		}
	}
	if (j == 0)
		divided[j++] = '0';

	divided[j] = '\0';
	strcpy(result, divided);
	return reminder;
}

int str_whole_divide_string(char* str1, char* str2, int base)
{
	char multiplied[STR_SIZE_DEF];
	int multiplier = 1;
	strcpy(multiplied, str2);
	while (str_is_greater(str1, multiplied) >= 0)
	{
		multiplier++;
		str_whole_multiply(str2, multiplied, base, multiplier);
	}
	return --multiplier;
}

int str_decimal_divide_string(char* str1, char* str2, int base)
{
	int len = max_i(strlen_i(str1), strlen_i(str2));
	char s1[STR_SIZE_DEF], s2[STR_SIZE_DEF], multiplied[STR_SIZE_DEF];
	sprintf(s1, "%-*s", len, str1), sprintf(s2, "%-*s", len, str2);
	str_pad_space(s1, '0'), str_pad_space(s2, '0');

	int multiplier = 1;
	strcpy(multiplied, s2);
	while (str_decimal_is_greater(s1, multiplied) >= 0)
	{
		multiplier++;
		int carry = str_decimal_multiply(s2, multiplied, base, multiplier);
		if (carry)
			break;
	}
	return --multiplier;
}

void ascnumber_clean(struct ascnumber* num)
{
	if (num->whole != NULL)
	{
		free(num->whole);
		num->whole = NULL;
	}
	if (num->decimal != NULL)
	{
		free(num->decimal);
		num->decimal = NULL;
	}
}

struct result str_to_ascnumber(const char* str, struct ascnumber* num)
{
	int len;
	if (str == NULL || (len = strlen_i(str)) == 0)
		return (struct result){.is_error=true, .message="Given number is an empty string\n" };

	unsigned int whole_offset = 0, decimal_offset = 0;
	unsigned int whole_size = 0, decimal_size = 0;

	if (str[0] == '-')
	{
		num->is_negative = true, num->used_sign_bit = false;
		whole_offset = 1;
	}
	else if (str[0] == '+')
	{
		num->is_negative = false, num->used_sign_bit = false;
		whole_offset = 1;
	}
	else if (len >= 2 && str[0] == '1' && str[1] == '.')
	{
		num->is_negative = true, num->used_sign_bit = true;
		whole_offset = 2;
	}
	else if (len >= 2 && str[0] == '0' && str[1] == '.')
	{
		num->is_negative = false, num->used_sign_bit = true;
		whole_offset = 2;
	}
	else
	{
		num->is_negative = false, num->used_sign_bit = false;
		whole_offset = 0;
	}

	int highest_digit = 0;
	for (int i = whole_offset; i < len; i++)
	{
		int value;
		if ((value = char_to_ascvalue(str[i])) != (int)-1)
		{
			highest_digit = max(highest_digit, value);
			whole_size++;
			continue;
		}
		if (str[i] == ',')
		{
			decimal_offset = i + 1;
			break;
		}	
		struct result res = { .is_error = true };
		sprintf(res.message, "Invalid character '%c' in number '%s'\n", str[i], str);
		return res;
	}
	if (whole_size == 0)
	{
		struct result res = { .is_error = true };
		sprintf(res.message, "Whole part of a number '%s' cannot be empty\n", str);
		return res;
	}

	num->whole = malloc_s((size_t)whole_size + 1);
	strncpy(num->whole, &str[whole_offset], whole_size);
	num->whole[whole_size] = '\0';

	if (decimal_offset == 0)
	{
		num->decimal = NULL;
		num->highest_digit = highest_digit;
		return (struct result) { .is_error = false };
	}

	for (int i = decimal_offset; i < len; i++)
	{
		int value;
		if ( (value = char_to_ascvalue(str[i])) != (int)-1)
		{
			highest_digit = max(highest_digit, value);
			decimal_size++;
			continue;
		}
		struct result res = { .is_error = true };
		sprintf(res.message, "Invalid character '%c' in number '%s'\n", str[i], str);
		return res;
	}
	if (decimal_size != 0)
	{
		num->decimal = malloc_s((size_t)decimal_size + 1);
		strncpy(num->decimal, &str[decimal_offset], decimal_size);
		num->decimal[decimal_size] = '\0';

	}

	num->highest_digit = highest_digit;
	return (struct result) { .is_error = false };
}

struct ascnumber str_convert(struct ascnumber number, int base_from, int base_to)
{
	char buff[STR_SIZE_DEF];
	struct ascnumber converted =
	{
		.is_negative = number.is_negative,
		.used_sign_bit = number.used_sign_bit,
		.whole = "0",
		.decimal = "0"
	};

	if (!str_is_zero(number.whole))
	{
		str_whole_convert(number.whole, buff, base_from, base_to);
		converted.whole = malloc_s(strlen_i(buff) + 1);
		strcpy(converted.whole, buff);
	} 
	if (!str_is_zero(number.decimal))
	{
		str_decimal_convert(number.decimal, buff, base_from, base_to);
		converted.decimal = malloc_s(strlen_i(buff) + 1);;
		strcpy(converted.decimal, buff);
	}

	return converted;
}

void str_whole_convert(char* str, char* result, int base_from, int base_to)
{
	char divided_str[STR_SIZE_DEF] = "", result_str[STR_SIZE_DEF] = "";
	int reminder = 0, i = 0;

	strcpy(divided_str, str);
	while (!str_is_zero(divided_str))
	{
		int reminder = str_whole_divide_constant(divided_str, divided_str, base_from, base_to);
		char reminder_ch = ascvalue_to_char(reminder);
		result_str[i] = reminder_ch;
		i++;
	}
	if (i == 0)
		result_str[i++] = '0';

	result_str[i] = '\0';
	str_reverse(result_str);
	strcpy(result, result_str);
}

void str_decimal_convert(char* str, char* result, int base_from, int base_to)
{
	if (str_is_zero(str))
	{
		result[0] = '0', result[1] = '\0';
		return;
	}

	char multiplied_str[STR_SIZE_DEF] = "", result_str[STR_SIZE_DEF] = "";
	double result_value = 0, str_value = str_deciaml_value(str, base_from);
	int reminder = 0, i = 0;

	strcpy(multiplied_str, str);
	do
	{
		reminder = str_decimal_multiply(multiplied_str, multiplied_str, base_from, base_to);
		result_str[i] = ascvalue_to_char(reminder);
		result_str[i + 1] = '\0';
		result_value = str_deciaml_value(result_str, base_to);
		i++;
	} while (i < STR_SIZE_DEF - 1 && !is_within(result_value, str_value, STR_PREC_DEF));

	result_str[i] = '\0';
	strcpy(result, result_str);
}

void str_expand(char* num, int whole_len, int decimal_len, char c)
{
	int len = strlen_i(num);
	char* pos = strchr(num, ',');
	int wl = (int)(pos - num);
	int dl = len - wl - 1;

	int wextra = whole_len - wl, dextra = decimal_len - dl;
	if (wextra > 0)
	{
		char* padding = malloc_s(wextra + 1);
		padding[wextra] = '\0';
		for (int i = 0; i < wextra; i++)
			padding[i] = c;

		strcat_b(num, padding);
		free(padding);
	}
	if (dextra > 0)
	{
		char* padding = malloc_s(dextra + 1);
		padding[dextra] = '\0';
		for (int i = 0; i < dextra; i++)
			padding[i] = c;

		strcat(num, padding);
		free(padding);
	}
}

void str_compliment_P(char* num, char* result, int base)
{
	int len = strlen_i(num);
	result[len] = '\0';

	for (int i = len - 1; i >= 0; i--)
	{
		if (num[i] == ',')
		{
			result[i] = num[i];
			continue;
		}
		if (num[i] == '.')
		{
			result[i] = num[i];
			result[i - 1] = num[i - 1] == '0' ? '1' : '0';
			break;
		}

		result[i] = ascvalue_to_char(base - 1 - char_to_ascvalue(num[i]));
	}
}

void str_compliment_Pm1(char* num, char* result, int base)
{
	if (num[0] == '1' && str_is_zero(&num[2]))
	{
		strcpy(result, num);
		result[0] = '0';
		char buff[2];
		sprintf(buff, "%c", ascvalue_to_char(base - 1));
		strcat_b(&result[2], buff);
		return;
	}

	str_compliment_P(num, result, base);
	int carry = 1;

	for (int i = strlen_i(result) - 1; i >= 0; i--)
	{
		if (result[i] == ',' || result[i] == '.')
			continue;

		int val = char_to_ascvalue(result[i]) + carry;
		result[i] = ascvalue_to_char(val % base);
		carry = val / base;
	}
}

int str_is_min_Pm1(struct ascnumber num)
{
	if (num.is_negative && num.used_sign_bit)
		return str_is_zero(num.whole) && str_is_zero(num.decimal);
	return 0;
}

int str_addnum(char* num1, char* num2, char* result, int base)
{
	int carry = 0, len = strlen_i(num1);
	result[len] = '\0';

	for (int i = len - 1; i >= 0; i--)
	{
		if (num1[i] == ',' || num1[i] == '.')
		{
			result[i] = num1[i];
			continue;
		}
		int sum = char_to_ascvalue(num1[i]) + char_to_ascvalue(num2[i]) + carry;
		result[i] = ascvalue_to_char(sum % base);
		carry = sum / base;
	}
	return carry;
}

int str_addconst(char* num, char* result, int base, int constant)
{
	int carry = constant, len = strlen_i(num);
	result[len] = '\0';

	for (int i = len - 1; i >= 0; i--)
	{
		if (num[i] == ',' || num[i] == '.')
		{
			result[i] = num[i];
			continue;
		}

		int val = char_to_ascvalue(num[i]) + carry;
		result[i] = ascvalue_to_char(val % base);
		carry = val / base;
	}

	return carry;
}

int str_addnum_bcd(char* num1, char* num2, char* result, struct tetrade* map)
{
	int carry = 0, len = strlen_i(num1);
	result[len] = '\0';

	for (int i = len - 1; i >= 0; i--)
	{
		if (num1[i] == ',')
		{
			result[i] = num1[i];
			continue;
		}
		if (num1[i] == '.')
		{
			result[i] = num1[i];
			int sum = char_to_ascvalue(num1[i - 1]) + char_to_ascvalue(num2[i - 1]) + carry;
			carry = sum / 2;
			result[i - 1] = ascvalue_to_char(sum % 2);
			break;
		}
		int sum = char_to_ascvalue(num1[i]) + char_to_ascvalue(num2[i]) + carry;
		result[i] = ascvalue_to_char(sum);
		carry = map[sum].carry;
	}
	return carry;
}

int str_addconst_bcd(char* num, char* result, struct tetrade* map, int constant)
{
	int carry = constant, len = strlen_i(num);
	result[len] = '\0';

	for (int i = len - 1; i >= 0; i--)
	{
		if (num[i] == ',' || num[i] == '.')
		{
			result[i] = num[i];
			continue;
		}

		int val = char_to_ascvalue(num[i]) + carry;
		result[i] = ascvalue_to_char(val);
		carry = map[val].carry;
	}

	return carry;
}