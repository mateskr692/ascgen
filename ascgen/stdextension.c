#pragma once
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "stdextension.h"

void* malloc_s(int size)
{
	void* mem = malloc(size);
	if (mem == NULL)
		exit(2);
	
	return mem;
}

int strlen_i(const char* str)
{
	return (int)strlen(str);
}

int pow_i(int base, int exponent)
{
	if (base == 0)
		return 0;

	int result = 1;
	for (int i = 0; i < exponent; i++)
		result *= base;

	return result;
}

int max_i(int a, int b)
{
	return a > b ? a : b;
}

int min_i(int a, int b)
{
	return a < b ? a : b;
}

int char_to_ascvalue(char c)
{
	if (c >= '0' && c <= '9')
		return c - 48;
	if (c >= 'A' && c <= 'Z')
		return c - 55;
	if (c >= 'a' && c <= 'z')
		return c - 87;

	return -1;
}

char ascvalue_to_char(int v)
{
	if (v < 10)
		return v + 48;

	return v + 55;
}

bool is_within(double value, double original, double error)
{
	double d = value / original;
	return d >= 1.0 - error && d <= 1.0 + error;
}

bool str_is_zero(char* str)
{
	for (int i = 0; i < strlen_i(str); i++)
	{
		if (str[i] != '0' && str[i] != ',')
			return false;
	}
	return true;
}

bool str_is_all(char* str, char c)
{
	for (int i = 0; i < strlen_i(str); i++)
	{
		if (str[i] != c && str[i] != ',')
			return false;
	}
	return true;
}

void strcat_b(char* dest, char* str)
{
	int offset = strlen_i(str);
	for (int i = strlen_i(dest); i >= 0; i--)
		dest[i + offset] = dest[i];
	for (int i = 0; i < offset; i++)
		dest[i] = str[i];
}

void str_reverse(char* str)
{
	int len = strlen_i(str);
	for (int i = 0; i < len / 2; i++)
	{
		char c = str[i];
		str[i] = str[len - 1 - i];
		str[len - 1 - i] = c;
	}
}

bool str_is_space(char* str)
{
	for (int i = 0; i < strlen_i(str); i++)
	{
		if (!isspace(str[i]))
			return false;
	}
	return true;
}

int str_value_at(char* str, int pos)
{
	if (pos >= strlen_i(str))
		return 0;
	return char_to_ascvalue(str[pos]);
}
