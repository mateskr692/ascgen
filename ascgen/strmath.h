#pragma once
#include <stdio.h>
#include <string.h>
#include "result.h"

#define STR_SIZE_DEF 50
#define STR_PREC_DEF 0.001

struct ascnumber
{
	char* whole;
	char* decimal;
	bool is_negative;
	bool used_sign_bit;
	int highest_digit;
};

struct tetrade
{
	char* code;
	char carry;
};

void ascnumber_clean(struct ascnumber* num);

int str_decimal_is_greater(char* s1, char* s2);
int str_is_greater(char* s1, char* s2);
void int_to_str(char* str, int num, int base);
struct result str_to_int(char* str, int* num, int base);
struct result str_to_ascnumber(const char* str, struct ascnumber* num);
struct ascnumber str_convert(struct ascnumber number, int base_from, int base_to);
void str_whole_convert(char* str, char* result, int base_from, int base_to);
void str_decimal_convert(char* str, char* result, int base_from, int base_to);

void str_whole_multiply(char* str, char* result, int base, int multiplier);
int str_decimal_multiply(char* str, char* result, int base, int multiplier);
int str_whole_divide_constant(char* str, char* result, int base, int divisor);
int str_whole_divide_string(char* str1, char* str2, int base);
int str_decimal_divide_string(char* str1, char* str2, int base);
void str_whole_subtract(char* str1, char* str2, char* result, int base);
void str_decimal_subtract(char* str1, char* str2, char* result, int base);
double str_deciaml_value(char* str, int base);
void str_pow(int base, int exponent, char* result, int result_base);

void str_expand(char* num, int whole_len, int decimal_len, char c);
void str_compliment_P(char* num, char* result, int base);
void str_compliment_Pm1(char* num, char* result, int base);
int str_is_min_Pm1(struct ascnumber num);
int str_addnum(char* num1, char* num2, char* result, int base);
int str_addconst(char* num, char* result, int base, int constant);
int str_addnum_bcd(char* num1, char* num2, char* result, struct tetrade* map);
int str_addconst_bcd(char* num, char* result, struct tetrade* map, int constant);
