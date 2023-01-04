#pragma once
#include <stdio.h>
#include <stdbool.h>
#include "result.h"

struct arg_option
{
	char* short_name;
	char* long_name;

	bool* occured;
	char** value;
	char* hint;

	char* description;
};

struct arg_option optv(char** value, char* hint);
struct arg_option optb(bool* occured);
struct arg_option names(char* s, char* l, struct arg_option opt);
struct arg_option description(char* desc, struct arg_option opt);

int is_option(char* str);


struct parser_settings
{
	struct arg_option* options;
	int options_size;

	char** unmatched;
	int* unmatched_count;
	char* unmatched_name;
};

struct result pars_args(int argc, char* argv[], struct parser_settings* settings);
void print_help(char* program_name, struct parser_settings* settings);