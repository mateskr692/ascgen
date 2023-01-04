#pragma once
#include <string.h>
#include "conversion.h"
#include "strmath.h"
#include "stdextension.h"

static struct result parse(struct convdata data, struct convdata_parsed* parsed)
{
	struct result r;
	parsed->precision = CONV_PREC_DEF;

	if (data.values_count == 0)
		return (struct result) { .is_error = true, .message = "no numbers provided for conversion\n" };
	if (data.base_from == NULL)
		return (struct result) { .is_error = true, .message = "no base specified for the numbers\n" };
	if (data.base_to == NULL)
		return (struct result) { .is_error = true, .message = "no base specified for the conversion\n" };

	r = str_to_int(data.base_from, &parsed->base_from, 10);
	if (r.is_error)
		return r;
	r = str_to_int(data.base_to, &parsed->base_to, 10);
	if (r.is_error)
		return r;
	if (data.precission != NULL)
	{
		r = str_to_int(data.precission, &parsed->precision, 10);
		if (r.is_error)
			return r;
	}

	parsed->numbers = malloc_s(data.values_count * sizeof(struct ascnumber));
	parsed->numbers_count = data.values_count;
	for (int i = 0; i < data.values_count; i++)
	{
		r = str_to_ascnumber(data.values[i], &parsed->numbers[i]);
		if (r.is_error)
			return r;

		if (parsed->numbers[i].highest_digit >= parsed->base_from)
		{
			struct result res = { .is_error = true };
			sprintf(res.message, "number '%s' conrains a digit '%c' but is of base '%d'", data.values[i], ascvalue_to_char(parsed->numbers[i].highest_digit), parsed->base_from);
			return res;
		}
	}

	return (struct result) { .is_error = false };
}

struct result convert_il(FILE* out_stream, struct convdata data)
{
	//parse data
	struct convdata_parsed parsed;
	struct result r = parse(data, &parsed);
	if (r.is_error)
		return r;

	int base_from = parsed.base_from, base_to = parsed.base_to;
	for (int ni = 0; ni < parsed.numbers_count; ni++)
	{
		struct ascnumber* num = &parsed.numbers[ni];
		char* sign = num->is_negative ? "-" : "";

		fprintf(out_stream, "Konwersja, metoda iloczynowo-ilorazowa:\n");
		if(num->decimal == NULL)
			fprintf(out_stream, "A_%d = %s%s\n\n", base_from, sign, num->whole);
		else
			fprintf(out_stream, "A_%d = %s%s,%s\n\n", base_from, sign, num->whole, num->decimal);

		int l1 = 0, l2 = 0, l3 = 0, l4 = 0;
		char b1[STR_SIZE_DEF], b2[STR_SIZE_DEF], b3[STR_SIZE_DEF], b4[STR_SIZE_DEF];
		char whole_str[STR_SIZE_DEF], decimal_str[STR_SIZE_DEF];
		char whole_converted[STR_SIZE_DEF] = "", decimal_converted[STR_SIZE_DEF] = "";
		strcpy(whole_str, num->whole);
		if(num->decimal != NULL)
			strcpy(decimal_str, num->decimal);

		//convert whole part
		int idx = 0;
		fprintf(out_stream, "C_%d = %s\n", base_from, num->whole);
		while (!str_is_zero(whole_str))
		{
			char divided_str[STR_SIZE_DEF], reminder_str[STR_SIZE_DEF];
			int reminder = str_whole_divide_constant(whole_str, divided_str, base_from, base_to);
			char reminder_ch = ascvalue_to_char(reminder);
			int_to_str(reminder_str, reminder, base_from);

			l1 = max_i(l1, sprintf(b1, "%s/%d", whole_str, base_to));
			l2 = max_i(l2, sprintf(b2, "%s", divided_str));
			l3 = max_i(l3, sprintf(b3, "%s/%d", reminder_str, base_to));
			l4 = max_i(l4, sprintf(b4, "a_%d", idx));
			fprintf(out_stream, "\t%*s = %*s + %*s -> %*s = %c\n", l1, b1, l2, b2, l3, b3, l4, b4, reminder_ch);
				
			strcpy(whole_str, divided_str);
			whole_converted[idx] = reminder_ch;
			idx++;
		}
		if (idx == 0)
			whole_converted[idx++] = '0';
		whole_converted[idx] = '\0';
		str_reverse(whole_converted);
		fprintf(out_stream, "C_%d = %s\n\n", base_to, whole_converted);
		if (num->decimal == NULL)
		{
			fprintf(out_stream, "A_%d = %s%s\n", base_to, sign, whole_converted);
			continue;
		}

		//convert decimal part
		idx = 0;
		l1 = 0, l2 = 0, l3 = 0, l4 = 0;
		fprintf(out_stream, "U_%d = 0,%s\n", base_from, num->decimal);
		while (!str_is_zero(decimal_str) && idx < parsed.precision)
		{
			char multiplied_str[STR_SIZE_DEF], overflow_str[STR_SIZE_DEF];
			int overflow = str_decimal_multiply(decimal_str, multiplied_str, base_from, base_to);
			int_to_str(overflow_str, overflow, base_from);
			char overflow_ch = ascvalue_to_char(overflow);

			l1 = max_i(l1, sprintf(b1, "0,%s*%d", decimal_str, base_to));
			l2 = max_i(l2, sprintf(b2, "0,%s", multiplied_str));
			l3 = max_i(l3, sprintf(b3, "%s", overflow_str));
			l4 = max_i(l4, sprintf(b4, "a_-%d", idx + 1));
			fprintf(out_stream, "\t%*s = %*s + %*s -> %*s = %c\n", l1, b1, l2, b2, l3, b3, l4, b4, overflow_ch);

			strcpy(decimal_str, multiplied_str);
			decimal_converted[idx] = overflow_ch;
			idx++;
		}
		if (idx == 0)
			decimal_converted[idx++] = '0';
		decimal_converted[idx] = '\0';
		fprintf(out_stream, "U_%d = 0,%s\n\n", base_to, decimal_converted);
		fprintf(out_stream, "A_%d = %s%s,%s\n\n", base_to, sign, whole_converted, decimal_converted);
	}

	struct result res = { .is_error = false };
	return res;
}

struct result convert_roz(FILE* out_stream, struct convdata data)
{	
	//parse data
	struct convdata_parsed parsed;
	struct result r = parse(data, &parsed);
	if (r.is_error)
		return r;

	int base_from = parsed.base_from, base_to = parsed.base_to;
	for (int ni = 0; ni < parsed.numbers_count; ni++)
	{
		struct ascnumber* num = &parsed.numbers[ni];
		char* sign = num->is_negative ? "-" : "";

		fprintf(out_stream, "Konwersja, metoda roznicowa:\n");
		if (num->decimal == NULL)
			fprintf(out_stream, "A_%d = %s%s\n\n", base_from, sign, num->whole);
		else
			fprintf(out_stream, "A_%d = %s%s,%s\n\n", base_from, sign, num->whole, num->decimal);

		int l1 = 0, l2 = 0, l3 = 0, l4 = 0, l5 = 0;
		char b1[STR_SIZE_DEF] = "", b2[STR_SIZE_DEF] = "", b3[STR_SIZE_DEF] = "", b4[STR_SIZE_DEF] = "", b5[STR_SIZE_DEF] ="";
		char whole_str[STR_SIZE_DEF] = "", decimal_str[STR_SIZE_DEF] = "0";
		char whole_converted[STR_SIZE_DEF] = "", decimal_converted[STR_SIZE_DEF] = "";
		strcpy(whole_str, num->whole);
		if(num->decimal != NULL)
			strcpy(decimal_str, num->decimal);

		//find the biggest exponent that fits in number
		int exponent = 0, idx = 0;
		strcpy(whole_converted, whole_str);
		while (1)
		{
			if (str_is_zero(whole_converted))
			{
				exponent--;
				break;
			}
			str_whole_divide_constant(whole_converted, whole_converted, base_from, base_to);
			exponent++;
		}

		//get alignment for decimal
		{
			char buff[STR_SIZE_DEF];
			sprintf(buff, "%*c", parsed.precision, '1');
			str_pad_space(buff, '0');
			str_decimal_convert(buff, buff, base_to, base_from);
			int len = sprintf(b1, "0,%s", buff);
			l1 = max_i(l1, len);
			l3 = max_i(l3, len);
			l4 = max_i(l4, len);
			len = sprintf(b2, "%d^%d * %d", base_to, -parsed.precision, base_to - 1);
			l2 = max_i(l2, len);
		}

		//convert whole
		while (exponent >= 0)
		{
			char power_str[STR_SIZE_DEF], multiplied_str[STR_SIZE_DEF], difference_str[STR_SIZE_DEF];
			str_pow(base_to, exponent, power_str, base_from);
			int multiplier = str_whole_divide_string(whole_str, power_str, base_from);
			char multiplier_ch = ascvalue_to_char(multiplier);
			if (multiplier != 0)
			{
				str_whole_multiply(power_str, multiplied_str, base_from, multiplier);
				str_whole_subtract(whole_str, multiplied_str, difference_str, base_from);

				l1 = max_i(l1, sprintf(b1, "%s,%s", whole_str, decimal_str));
				l2 = max_i(l2, sprintf(b2, "%d^%d * %d", base_to, exponent, multiplier));
				l3 = max_i(l3, sprintf(b3, "%s", multiplied_str));
				l4 = max_i(l4, sprintf(b4, "%s,%s", difference_str, decimal_str));
				l5 = max_i(l5, sprintf(b5, "a_%d", exponent));
				fprintf(out_stream, "\t%*s - %-*s = %*s - %*s = %*s > 0 -> %*s = %c\n", l1, b1, l2, b2, l1, b1, l3, b3, l4, b4, l5, b5, multiplier_ch);
			}
			else
			{
				strcpy(difference_str, whole_str);
				strcpy(multiplied_str, power_str);

				l1 = max_i(l1, sprintf(b1, "%s,%s", whole_str, decimal_str));
				l2 = max_i(l2, sprintf(b2, "%d^%d", base_to, exponent));
				l3 = max_i(l3, sprintf(b3, "%s", multiplied_str));
				l4 = max_i(l4, sprintf(b4, ""));
				l5 = max_i(l5, sprintf(b5, "a_%d", exponent));
				fprintf(out_stream, "\t%*s - %-*s = %*s - %*s   %*s < 0 -> %*s = %c\n", l1, b1, l2, b2, l1, b1, l3, b3, l4, b4, l5, b5, multiplier_ch);
			}
			strcpy(whole_str, difference_str);
			whole_converted[idx] = multiplier_ch;
			idx++;
			exponent--;
		}
		if (idx == 0)
			whole_converted[idx++] = '0';
		whole_converted[idx] = '\0';

		if (num->decimal == NULL)
		{
			fprintf(out_stream, "A_%d = %s%s\n", base_to, sign, whole_converted);
			continue;
		}

		//convert decimal
		exponent = 1;
		while (!str_is_zero(decimal_str) && exponent <= parsed.precision)
		{
			char fraction_str[STR_SIZE_DEF], multiplied_str[STR_SIZE_DEF], difference_str[STR_SIZE_DEF];
			sprintf(fraction_str, "%*c", exponent, '1');
			str_pad_space(fraction_str, '0');
			str_decimal_convert(fraction_str, fraction_str, base_to, base_from);
			int multiplier = str_decimal_divide_string(decimal_str, fraction_str, base_from);
			char multiplier_ch = ascvalue_to_char(multiplier);
			if (multiplier != 0)
			{
				str_whole_multiply(fraction_str, multiplied_str, base_from, multiplier);
				str_decimal_subtract(decimal_str, multiplied_str, difference_str, base_from);

				l1 = max_i(l1, sprintf(b1, "0,%s", decimal_str));
				l2 = max_i(l2, sprintf(b2, "%d^%d * %d", base_to, -exponent, multiplier));
				l3 = max_i(l3, sprintf(b3, "0,%s", multiplied_str));
				l4 = max_i(l4, sprintf(b4, "0,%s", difference_str));
				l5 = max_i(l5, sprintf(b5, "a_%d", -exponent));
				fprintf(out_stream, "\t%*s - %-*s = %*s - %*s = %*s > 0 -> %*s = %c\n", l1, b1, l2, b2, l1, b1, l3, b3, l4, b4, l5, b5, multiplier_ch);
			}
			else
			{
				strcpy(difference_str, decimal_str);
				strcpy(multiplied_str, fraction_str);

				l1 = max_i(l1, sprintf(b1, "0,%s", decimal_str));
				l2 = max_i(l2, sprintf(b2, "%d^%d", base_to, -exponent));
				l3 = max_i(l3, sprintf(b3, "0,%s", multiplied_str));
				l4 = max_i(l4, sprintf(b4, ""));
				l5 = max_i(l5, sprintf(b5, "a_%d", -exponent));
				fprintf(out_stream, "\t%*s - %-*s = %*s - %*s   %*s < 0 -> %*s = %c\n", l1, b1, l2, b2, l1, b1, l3, b3, l4, b4, l5, b5, multiplier_ch);
			}
			strcpy(decimal_str, difference_str);
			decimal_converted[exponent-1] = multiplier_ch;
			exponent++;
		}
		if(exponent == 1)
			decimal_converted[1 - exponent++] = '0';
		decimal_converted[exponent-1] = '\0';
		fprintf(out_stream, "\nA_%d = %s%s,%s\n\n", base_to, sign, whole_converted, decimal_converted);
	}

	struct result res = { .is_error = false };
	return res;
}
