#pragma once
#include <string.h>
#include <stdlib.h>
#include "parsing.h"
#include "stdextension.h"
#define OPT_WIDTH 40

struct arg_option optv(char** value, char* hint) 
{
	struct arg_option o = { .value = value, .hint = hint };
	return o;
}
struct arg_option optb(bool* occured)
{
	struct arg_option o = { .occured = occured };
	return o;
}
struct arg_option names(char* s, char* l, struct arg_option opt)
{
	opt.short_name = s;
	opt.long_name = l;
	return opt;
}
struct arg_option description(char* desc, struct arg_option opt)
{
	opt.description = desc;
	return opt;
}

//starts with either -, --, /
//0,1,2,3 => N/A,-,/,--
int is_option(char* str)
{
	if (str == NULL)
		return 0;

	int len = strlen_i(str);
	if (len > 1 && str[0] == '/')
		return 2;
	if (len > 2 && str[0] == '-' && str[1] == '-')
		return 2;
	if (len > 1 && str[0] == '-')
		return 1;

	return 0;

}

struct result pars_args(int argc, char* argv[], struct parser_settings* settings)
{
	*settings->unmatched_count = 0;
	if (argc <= 1)
		return (struct result) { .is_error = false };

	for (int i = 1; i < argc; i++)
	{
		int is_opt = is_option(argv[i]);
		if (is_opt == 0)
		{
			settings->unmatched[(*settings->unmatched_count)++] = argv[i];
			continue;
		}

		int offset = 1 + (is_opt == 3);
		bool match_short = is_opt < 3;
		bool matched = false;

		for (int j = 0; j < settings->options_size; j++)
		{
			struct arg_option* opt = &settings->options[j];

			if (opt->long_name != NULL)
			{
				if (strcmp(opt->long_name, &argv[i][offset]) == 0)
					matched = true;
			}
			if (!matched && match_short && opt->short_name != NULL)
			{
				if (strcmp(opt->short_name, &argv[i][offset]) == 0)
					matched = true;
			}

			if (matched)
			{
				if (opt->occured != NULL)
				{
					*opt->occured = true;
					break;
				}
				if (i + 1 >= argc || is_option(argv[i+1]) != 0)
				{
					struct result res = { .is_error = true };
					sprintf(res.message, "No argument for option '%s'\n", argv[i]);
					return res;
				}

				*opt->value = argv[++i];
				break;
			}
		}

		if (!matched)
		{
			settings->unmatched[(*settings->unmatched_count)++] = argv[i];
			/*struct result res = {.is_error = true};
			sprintf(res.message, "Invalid option '%s'\n", argv[i]);
			return res;*/
		}
	}

	return (struct result) { .is_error = false };
}

void print_help(char* program_name, struct parser_settings* settings)
{
	int w = printf("\nUsage: %s ", program_name) - 1;
	int printed_max = 25;
	int printed = 0;

	for (int i = 0; i < settings->options_size; i++)
	{ 
		if (printed >= printed_max)
		{
			printed = 0;
			printf("\n%*s", w, "");
		}

		struct arg_option* opt = &settings->options[i];

		char* name = opt->short_name == NULL ? opt->long_name : opt->short_name;
		char* special = opt->short_name == NULL ? "--" : "-";

		if (opt->occured == NULL)
			printed += printf("[%s%s <%s>] ", special, name, settings->options[i].hint);
		else
			printed += printf("[%s%s] ", special, name);
	}
	if (settings->unmatched_name != NULL)
		printf(" %s", settings->unmatched_name);


	printf("\n\nOptions:");
	for (int i = 0; i < settings->options_size; i++)
	{
		struct arg_option* opt = &settings->options[i];
		
		char str[OPT_WIDTH] = "";
		char buffor[OPT_WIDTH];

		if (opt->short_name != NULL)
			snprintf(str, OPT_WIDTH, "-%s, ", opt->short_name);

		snprintf(buffor, OPT_WIDTH, "--%s", opt->long_name);
		strcat(str, buffor);

		if (opt->occured == NULL)
		{
			snprintf(buffor, OPT_WIDTH, " <%s>", opt->hint);
			strcat(str, buffor);
		}
		printf("\n  %-*s", OPT_WIDTH, str);


		char* descr = malloc_s(strlen_i(opt->description) + 1);
		strcpy(descr, opt->description);

		char* substr = strtok(descr, "\n");
		printf("%s", substr);
		substr = strtok(NULL, "\n");
		while (substr != NULL)
		{
			printf("\n  %-*s%s", OPT_WIDTH, "", substr);
			substr = strtok(NULL, "\n");
		}
		free(descr);
	}
	printf("\n");
}