#pragma once
#include <stdbool.h>
#include "parsing.h"

struct mainargs
{
	int argc;
	char** argv;
};

struct program_config
{
	char* input_file;
	char* output_file;
	char* mode;
	char* base;
	char* base_to;
	char* precision;
	char* significant;
	bool  show_help;
	bool  erase;

	char** values;
	int values_count;
};

enum program_mode
{
	INVALID,

	k_il,
	k_roz,
	d_mod,
	d_dop,
	d_odw,
	d_mod_8421,
	d_dop_8421,
	d_odw_8421,
	d_mod_ex3,
	d_dop_ex3,
	d_odw_ex3,
	m_bezp,
	//m_booth,
	m_bgn,
	m_rob,
	dz_nres,
};

struct result execute(struct mainargs arg);
struct result execute_from_file(FILE* file);
enum program_mode get_mode(char* str);
char* get_modes_description();
struct result read_args(FILE* f, struct mainargs* args, bool* endfile);
void clean_args(struct mainargs* args);