#pragma once
#include <stdlib.h>
#include "stdextension.h"
#include "shared.h"

void example_printline(FILE* stream, int offset, int length)
{
	for (int i = 0; i < offset; i++)
		fprintf(stream, " ");
	for (int i = 0; i < length; i++)
		fprintf(stream, "-");
	fprintf(stream, "\n");
}

void example_printcarry(FILE* stream, int offset, int length)
{
	for (int i = 0; i < offset; i++)
		fprintf(stream, " ");
	fprintf(stream, "%c", 192);

	for (int i = 0; i < length - 4; i++)
		fprintf(stream, "%c", 196);
	fprintf(stream, "> 1 +\n");
}

void mathadd_prepdata_mod(struct ascnumber num1, struct ascnumber num2, int base, char** n1, char** n2, char** res)
{
	int whole_len = max_i(strlen_i(num1.whole), strlen_i(num2.whole));
	int decimal_len = max_i(strlen_i(num1.decimal), strlen_i(num2.decimal));
	*n1 = malloc_s((size_t)whole_len + decimal_len + 2);
	*n2 = malloc_s((size_t)whole_len + decimal_len + 2);
	*res = malloc_s((size_t)whole_len + decimal_len + 3);

	sprintf(*n1, "%s,%s", num1.whole, num1.decimal);
	sprintf(*n2, "%s,%s", num2.whole, num2.decimal);
	str_expand(*n1, whole_len, decimal_len, '0');
	str_expand(*n2, whole_len, decimal_len, '0');
}

void mathadd_prepdata(struct ascnumber num1, struct ascnumber num2, int base, char** n1, char** n2, char** res, bool isU2)
{
	void (*compl_fun)(char*, char*, int) = isU2 ? str_compliment_Pm1 : str_compliment_P;
	int w1_corr = isU2 ? str_is_min_Pm1(num1) : 0;
	int w2_corr = isU2 ? str_is_min_Pm1(num1) : 0;
	int whole_len = max_i(strlen_i(num1.whole) + w1_corr, strlen_i(num2.whole) + w2_corr);
	int decimal_len = max_i(strlen_i(num1.decimal), strlen_i(num2.decimal));
	*n1 = malloc_s(whole_len + decimal_len + 5);
	*n2 = malloc_s(whole_len + decimal_len + 5);
	*res = malloc_s(whole_len + decimal_len + 5);

	sprintf(*n1, "%d.%s,%s", num1.is_negative && num1.used_sign_bit ? 1 : 0, num1.whole, num1.decimal);
	if (num1.used_sign_bit && num1.is_negative)
		compl_fun(*n1, *n1, base);
	str_expand(&(*n1)[2], whole_len, decimal_len, '0');
	if (num1.is_negative)
		compl_fun(*n1, *n1, base);

	sprintf(*n2, "%d.%s,%s", num2.is_negative && num2.used_sign_bit ? 1 : 0, num2.whole, num2.decimal);
	if (num2.used_sign_bit && num2.is_negative)
		compl_fun(*n2, *n2, base);
	str_expand(&(*n2)[2], whole_len, decimal_len, '0');
	if (num2.is_negative)
		compl_fun(*n2, *n2, base);
}

void mathmult_prepdata_mod(struct ascnumber num1, struct ascnumber num2, struct multdata* d)
{
	d->shift1 = str_is_zero(num1.whole) ? 0 : strlen_i(num1.whole);
	d->shift2 = str_is_zero(num2.whole) ? 0 : strlen_i(num2.whole);
	int l1 = d->shift1 + strlen_i(num1.decimal);
	int l2 = d->shift2 + strlen_i(num2.decimal);
	d->numlen = max_i(l1, l2);

	d->num1 = malloc_s((size_t)d->numlen + 5);
	d->num2 = malloc_s((size_t)d->numlen + 5);
	d->result = malloc_s((size_t)d->numlen * 2 + 5);

	sprintf(d->num1, "%d.%s,%s", num1.is_negative ? 1 : 0, num1.whole, num1.decimal);
	sprintf(d->num2, "%d.%s,%s", num2.is_negative ? 1 : 0, num2.whole, num2.decimal);
	str_expand(&(d->num1[2]), 0, d->numlen - d->shift1, '0');
	str_expand(&(d->num2[2]), 0, d->numlen - d->shift2, '0');
}

void mathmult_prepdata(struct ascnumber num1, struct ascnumber num2, struct multdata* d)
{
	if (num1.is_negative && num1.used_sign_bit)
		d->shift1 = str_is_all(num1.whole, '1') ? 0 : strlen_i(num1.whole);
	else
		d->shift1 = str_is_zero(num1.whole) ? 0 : strlen_i(num1.whole);
	if (num2.is_negative && num2.used_sign_bit)
		d->shift2 = str_is_all(num2.whole, '1') ? 0 : strlen_i(num2.whole);
	else
		d->shift2 = str_is_zero(num2.whole) ? 0 : strlen_i(num2.whole);
	
	d->shift1 += str_is_min_Pm1(num1);
	d->shift2 += str_is_min_Pm1(num2);
	int l1 = d->shift1 + strlen_i(num1.decimal);
	int l2 = d->shift2 + strlen_i(num2.decimal);
	d->numlen = max_i(l1, l2);
	d->num1 = malloc_s((size_t)d->numlen + 5);
	d->num2 = malloc_s((size_t)d->numlen + 5);
	d->result = malloc_s((size_t)d->numlen * 2 + 5);

	sprintf(d->num1, "%d.%s,%s", num1.is_negative && num1.used_sign_bit ? 1 : 0, num1.whole, num1.decimal);
	if (num1.used_sign_bit && num1.is_negative)
		str_compliment_Pm1(d->num1, d->num1, 2);
	str_expand(&(d->num1[2]), 0, d->numlen - d->shift1, '0');
	if (num1.is_negative)
		str_compliment_Pm1(d->num1, d->num1, 2);

	sprintf(d->num2, "%d.%s,%s", num2.is_negative && num2.used_sign_bit ? 1 : 0, num2.whole, num2.decimal);
	if (num2.used_sign_bit && num2.is_negative)
		str_compliment_Pm1(d->num2, d->num2, 2);
	str_expand(&(d->num2[2]), 0, d->numlen - d->shift2, '0');
	if (num2.is_negative)
		str_compliment_Pm1(d->num2, d->num2, 2);
}

void mathdiv_prepdata(struct ascnumber num1, struct ascnumber num2, struct multdata* d, int precision)
{
	if (num1.is_negative && num1.used_sign_bit)
		d->shift1 = str_is_all(num1.whole, '1') ? 0 : strlen_i(num1.whole);
	else
		d->shift1 = str_is_zero(num1.whole) ? 0 : strlen_i(num1.whole);
	if (num2.is_negative && num2.used_sign_bit)
		d->shift2 = str_is_all(num2.whole, '1') ? 0 : strlen_i(num2.whole);
	else
		d->shift2 = str_is_zero(num2.whole) ? 0 : strlen_i(num2.whole);

	d->shift1 += str_is_min_Pm1(num1);
	d->shift2 += str_is_min_Pm1(num2);
	d->shift1 = d->shift2 = max_i(d->shift1, d->shift2);
	int l1 = d->shift1 + strlen_i(num1.decimal);
	int l2 = d->shift2 + strlen_i(num2.decimal);
	d->numlen = max_i(l1, l2);
	d->num1 = malloc_s((size_t)d->numlen + 5);
	d->num2 = malloc_s((size_t)d->numlen + 5);

	int reslen = precision == 0 ? d->numlen : precision;
	d->result = malloc_s((size_t)reslen * 2 + 5);
	d->correction = malloc_s((size_t)reslen + 5);
	d->reminder = malloc_s((size_t)reslen * 2 + 5);


	sprintf(d->num1, "%d.%s,%s", num1.is_negative && num1.used_sign_bit ? 1 : 0, num1.whole, num1.decimal);
	if (num1.used_sign_bit && num1.is_negative)
		str_compliment_Pm1(d->num1, d->num1, 2);
	str_expand(&(d->num1[2]), d->shift1, d->numlen - d->shift1, '0');
	if (num1.is_negative)
		str_compliment_Pm1(d->num1, d->num1, 2);

	sprintf(d->num2, "%d.%s,%s", num2.is_negative && num2.used_sign_bit ? 1 : 0, num2.whole, num2.decimal);
	if (num2.used_sign_bit && num2.is_negative)
		str_compliment_Pm1(d->num2, d->num2, 2);
	str_expand(&(d->num2[2]), d->shift2, d->numlen - d->shift2, '0');
	if (num2.is_negative)
		str_compliment_Pm1(d->num2, d->num2, 2);
}

void mathmult_shift_result(char* result, int shift)
{
	int len = strlen_i(result);
	char* tmp = malloc_s((size_t)len + 2);

	tmp[0] = result[0], tmp[1] = '.';
	if (shift == 0)
	{
		tmp[2] = result[0], tmp[3] = ',';
		strcpy(&tmp[4], &result[2]);
		strcpy(result, tmp);
		return;
	}

	strcpy(&tmp[2], &result[2]);
	tmp[2 + shift] = ',';
	strcpy(&tmp[3 + shift], &result[2 + shift]);
	strcpy(result, tmp);

	free(tmp);
}

void mathmult_shift_left(char* num, int shift)
{
	if (shift == 0)
	{
		num[2] = num[0];
		num[0] = num[1] = ' ';
		str_trim_leading(num, num, ' ');
	}
	else
	{
		char* pos = strchr(num, ',');
		char* buff = malloc_s(strlen_i(pos));
		strcpy(buff, &pos[1]);
		strcpy(pos, buff);
		free(buff);
	}
	num[1] = ',';
}

struct result mathdata_parse(struct mathdata data, struct mathdata_parsed* parsed, int defualt_base)
{
	struct result r;

	if (data.values_count == 0)
		return (struct result) { .is_error = true, .message = "no numbers provided for operation\n" };

	if (data.base == NULL)
		parsed->base = defualt_base;
	else
	{
		r = str_to_int(data.base, &parsed->base, 10);
		if (r.is_error)
			return r;
	}

	if (data.precision == NULL)
		parsed->precision = 0;
	else
	{
		r = str_to_int(data.precision, &parsed->precision, 10);
		if (r.is_error)
			return r;
	}

	if (data.values_count != 2)
	{
		struct result res = { .is_error = true };
		sprintf(res.message, "operation requires exactly 2 numbers (%d provided)\n", data.values_count);
		return res;
	}

	struct ascnumber n1, n2;
	r = str_to_ascnumber(data.values[0], &n1);
	if (r.is_error)
		return r;
	r = str_to_ascnumber(data.values[1], &n2);
	if (r.is_error)
		return r;

	if (n1.highest_digit >= parsed->base)
	{
		struct result res = { .is_error = true };
		sprintf(res.message, "number '%s' conrains a digit '%c' but is of base '%d'", data.values[0], ascvalue_to_char(n1.highest_digit), parsed->base);
		return res;
	}
	if (n2.highest_digit >= parsed->base)
	{
		struct result res = { .is_error = true };
		sprintf(res.message, "number '%s' conrains a digit '%c' but is of base '%d'", data.values[1], ascvalue_to_char(n2.highest_digit), parsed->base);
		return res;
	}

	if (n1.decimal == NULL)
		n1.decimal = "0";
	if (n2.decimal == NULL)
		n2.decimal = "0";

	if (n1.is_negative && !n1.used_sign_bit && str_is_zero(n1.whole) && str_is_zero(n1.decimal))
		n1.is_negative = false;
	if (n2.is_negative && !n2.used_sign_bit && str_is_zero(n2.whole) && str_is_zero(n2.decimal))
		n2.is_negative = false;

	parsed->num1 = n1;
	parsed->num2 = n2;
	return (struct result) { .is_error = false };
}