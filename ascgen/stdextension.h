#pragma once
#include <stdbool.h>

void* malloc_s(int size);
int strlen_i(const char* str);
int pow_i(int base, int exponent);
int max_i(int a, int b);
int min_i(int a, int b);
int char_to_ascvalue(char c);
char ascvalue_to_char(int v);
bool is_within(double value, double original, double error);
bool str_is_zero(char* str);
bool str_is_all(char* str, char c);
void strcat_b(char* dest, char* str);
void str_reverse(char* str);
void str_trim_leading(char* str, char* res, char c);
void str_pad_space(char* str, char c);
bool str_is_space(char* str);
int str_value_at(char* str, int pos);