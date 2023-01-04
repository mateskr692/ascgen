#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "stdextension.h"
#include "parsing.h"
#include "program.h"
#include "result.h"
#include "conversion.h"
#include "addbin.h"
#include "add8421.h"
#include "addex3.h"
#include "multbezp.h"
#include "multbgn.h"
#include "multrob.h"
#include "divnr.h"

char* get_modes_description()
{
	return "tryb dzialania aplikacji, dotepne tryby:\n"
		"  k_il - konwersja iloczynowo-ilorazowa\n"
		"  k_roz - konwersja roznicowa\n"
		"  d_mod - dodawanie dwojkowe modulowe\n"
		"  d_dop - dodawanie dwojkowe dopelnieniowe\n"
		"  d_odw - dodawanie dwojkowe odwrorne\n"
		"  d_mod_8421 - dodawanie dziesietne modulowe w kodzie BCD\n"
		"  d_dop_8421 - dodawanie dziesietne dopelnieniowe w kodzie BCD\n"
		"  d_odw_8421 - dodawanie dziesietne odwrotne w kodzie BCD\n"
		"  d_mod_ex3 - dodawanie dziesietne modulowe w kodzie Excess-3\n"
		"  d_dop_ex3 - dodawanie dziesietne dopelnieniowe w kodzie Excess-3\n"
		"  d_odw_ex3 - dodawanie dziesietne odwrotne w kodzie Excess-3\n"
		"  m_bezp - mnozenie bezposrednia\n"
		"  m_bgn - mnozenie Burks-Goldstein-Neumann\n"
		"  m_rob - mnozenie Robertsona\n"
		"  dz_nres - dzielenie nierestytucyjna";
}

enum program_mode get_mode(char* str)
{
	if (strcmp(str, "k_il") == 0)
		return k_il;
	else if (strcmp(str, "k_roz") == 0)
		return k_roz;
	else if (strcmp(str, "d_mod") == 0)
		return d_mod;
	else if (strcmp(str, "d_dop") == 0)
		return d_dop;
	else if (strcmp(str, "d_odw") == 0)
		return d_odw;
	else if (strcmp(str, "d_mod_8421") == 0)
		return d_mod_8421;
	else if (strcmp(str, "d_dop_8421") == 0)
		return d_dop_8421;
	else if (strcmp(str, "d_odw_8421") == 0)
		return d_odw_8421;
	else if (strcmp(str, "d_mod_ex3") == 0)
		return d_mod_ex3;
	else if (strcmp(str, "d_dop_ex3") == 0)
		return d_dop_ex3;
	else if (strcmp(str, "d_odw_ex3") == 0)
		return d_odw_ex3;
	else if (strcmp(str, "m_bezp") == 0)
		return m_bezp;
	else if (strcmp(str, "m_bgn") == 0)
		return m_bgn;
	else if (strcmp(str, "m_rob") == 0)
		return m_rob;
	else if (strcmp(str, "dz_nres") == 0)
		return dz_nres;
	else
		return INVALID;
}

struct result read_args(FILE* f, struct mainargs* args, bool* endfile)
{
	char buff[500];
	while (true)
	{
		if (fgets(buff, 500, f) == NULL)
		{
			if (feof(f))
			{
				*endfile = true;
				return (struct result) { .is_error = false };
			}
			return (struct result) { .is_error = true, .message = "Failed to read data from input file\n" };
		}

		if (!str_is_space(buff))
			break;
	}

	//trim white characters at the end
	for (int i = strlen_i(buff) - 1; i >= 0; i--)
	{
		if (isspace(buff[i]))
			buff[i] = '\0';
		else
			break;
	}

	//count args
	args->argc = 2;
	for (int i = 0; i < strlen_i(buff); i++)
	{
		if (isspace(buff[i]))
		{
			args->argc++;
			buff[i] = ' ';
			while (isspace(buff[i + 1]))
				buff[1 + i++] = ' ';
		}
	}

	args->argv = malloc_s(sizeof(char*) * ((size_t)args->argc + 1));
	args->argv[0] = malloc_s(1);
	args->argv[0][0] = '\0';

	//seperate args
	char* tok = strtok(buff, " ");
	int idx = 1;
	while (tok)
	{
		args->argv[idx] = malloc_s(strlen_i(tok) + 1);
		strcpy(args->argv[idx++], tok);
		tok = strtok(NULL, " ");
	}

	return (struct result) { .is_error = false };
}

struct result execute_from_file(FILE* file)
{
	bool endfile = false;
	struct mainargs args;
	while (true)
	{
		struct result res = read_args(file, &args, &endfile);
		if (endfile)
			break;

		if (res.is_error)
			return res;

		res = execute(args);
		clean_args(&args);
		if (res.is_error)
			return res;
	}

	return (struct result) { .is_error = false };
}

void clean_args(struct mainargs* args)
{
	for (int i = 0; i < args->argc; i++)
		free(args->argv[i]);
	free(args->argv);
}

struct result execute(struct mainargs arg)
{
	struct program_config cfg =
	{
		.values = malloc_s(arg.argc * sizeof(char*)),
		.show_help = false,
		.erase = false
	};
	struct parser_settings settings =
	{
		.options = (struct arg_option[])
		{
			names("t","tryb",
			description(get_modes_description(),
			optv(&cfg.mode, "tryb"))),

			names("p","podstawa",
			description("podstawa podanej liczby",
			optv(&cfg.base, "liczba"))),

			names("n","na",
			description("podstawa na ktora nalezy dokonac konwersji",
			optv(&cfg.base_to, "liczba"))),

			names("d","dokladnosc",
			description("maksymalna liczba miejsc po przecinku do ktorych dokonywana jest konwersja",
			optv(&cfg.precision, "liczba"))),

			names("i","input",
			description("plik wejsciowy z ktorego kazda linia jest traktowana jako osobne argumenty dla programu",
			optv(&cfg.input_file, "plik wejsciowy"))),

			names("o","output",
			description("plik wyjsciowy do ktorego zostana zapisane przyklady",
			optv(&cfg.output_file, "plik wyjsciowy"))),

			names("e","erase",
			description("jezeli plik wyjsciowy istnieje, zostanie wyczyszczony (domyslnie dopisywane na koniec)",
			optb(&cfg.erase))),

			names("h","help",
			description("wyswietla instrukcje",
			optb(&cfg.show_help))),
		},
		.options_size = 8,

		.unmatched = cfg.values,
		.unmatched_count = &cfg.values_count,
		.unmatched_name = "liczby"
	};
	struct result pars_result = pars_args(arg.argc, arg.argv, &settings);

	if (cfg.show_help)
	{
		print_help("ascgen", &settings);
		return (struct result) { .is_error = false };
	}
	if (cfg.input_file)
	{
		FILE* file = fopen(cfg.input_file, "r");
		if (file == NULL)
			return (struct result) { .is_error = true, .message = "Failed to open input file\n" };
		
		struct result res = execute_from_file(file);
		fclose(file);
		return res;
	}
	if (pars_result.is_error)
		return pars_result;
	if (cfg.mode == NULL)
		return (struct result) { .is_error = true, .message = "No required option '--tryb' specified\n" };

	bool using_outfile = false;
	FILE* outstream = stdout;
	struct result main_result;
	struct convdata conv_data =
	{
		.base_from = cfg.base,
		.base_to = cfg.base_to,
		.values = cfg.values,
		.values_count = cfg.values_count,
		.precission = cfg.precision,
	};
	struct mathdata math_data =
	{
		.base = cfg.base,
		.values = cfg.values,
		.values_count = cfg.values_count,
		.precision = cfg.precision
	};

	if (cfg.output_file)
	{
		char* mode = cfg.erase ? "w" : "a";
		outstream = fopen(cfg.output_file, mode);
		using_outfile = true;
		if (outstream == NULL)
			return (struct result) { .is_error = true, .message = "Failed to open output file\n" };
	}


	switch (get_mode(cfg.mode))
	{
		case k_il:
			main_result = convert_il(outstream, conv_data); break;
		case k_roz:
			main_result = convert_roz(outstream, conv_data); break;
		case d_mod:
			main_result = addbin_mod(outstream, math_data); break;
		case d_odw:
			main_result = addbin_odw(outstream, math_data); break;
		case d_dop:
			main_result = addbin_dop(outstream, math_data); break;
		case d_mod_8421:
			main_result = add8421_mod(outstream, math_data); break;
		case d_odw_8421:
			main_result = add8421_odw(outstream, math_data); break;
		case d_dop_8421:
			main_result = add8421_dop(outstream, math_data); break;
		case d_mod_ex3:
			main_result = addex3_mod(outstream, math_data); break;
		case d_odw_ex3:
			main_result = addex3_odw(outstream, math_data); break;
		case d_dop_ex3:
			main_result = addex3_dop(outstream, math_data); break;
		case m_bezp:
			main_result = multbezp(outstream, math_data); break;
		case m_bgn:
			main_result = multbgn(outstream, math_data); break;
		case m_rob:
			main_result = multrob(outstream, math_data); break;
		case dz_nres:
			main_result = divnr(outstream, math_data); break;

		case INVALID:
		default:
			main_result.is_error = true;
			sprintf(main_result.message, "Invalid mode '%s'", cfg.mode);
	}

	if (using_outfile)
		fclose(outstream);

	if (main_result.is_error)
		return main_result;

	return (struct result) { .is_error = false };
}