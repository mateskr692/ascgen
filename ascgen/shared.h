#pragma once
#include "strmath.h"

struct mathdata
{
	char* base;
	char* precision;
	char** values;
	int values_count;
};

struct mathdata_parsed
{
	int base;
	int precision;
	struct ascnumber num1, num2;
};

struct convdata
{
	char* base_from;
	char* base_to;
	char* precission;
	char** values;
	int values_count;
};

struct convdata_parsed
{
	int base_from;
	int base_to;
	int precision;

	struct ascnumber* numbers;
	int numbers_count;
};

struct multdata
{
	char* num1, * num2, * result;
	int shift1, shift2;
	int numlen;
	char** partial_sums;
	
	char *correctS, *correctX, *correctY;
	char *reminder, *correction;

};

void example_printline(FILE* stream, int offset, int length);
void example_printcarry(FILE* stream, int offset, int length);
void mathadd_prepdata_mod(struct ascnumber num1, struct ascnumber num2, int base, char** n1, char** n2, char** res);
void mathadd_prepdata(struct ascnumber num1, struct ascnumber num2, int base, char** n1, char** n2, char** res, bool isU2);
void mathdiv_prepdata(struct ascnumber num1, struct ascnumber num2, struct multdata* d, int precision);
void mathmult_prepdata_mod(struct ascnumber num1, struct ascnumber num2, struct multdata* d);
void mathmult_prepdata(struct ascnumber num1, struct ascnumber num2, struct multdata* d);
void mathmult_shift_result(char* result, int shift);
void mathmult_shift_left(char* num, int shift);

struct result mathdata_parse(struct mathdata data, struct mathdata_parsed* parsed, int defualt_base);