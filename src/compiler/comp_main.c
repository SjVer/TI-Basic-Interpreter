#include <argp.h>
#include "comp_common.h"
#include "compiler.h"
#include "tools.h"
#include "cwalk.h"

const char *argp_program_version = COMP_NAME " " COMP_VERSION;
const char *argp_program_bug_address = EMAIL;

#define ARGS_COUNT 1

/* This structure is used by main to communicate with parse_opt. */
struct arguments
{
	char *args[ARGS_COUNT];	 /* FILE */
	bool verbose;			 /* The -v flag */
	char *outfile;			 /* Argument for -o */
	// char *string1, *string2; /* Arguments for -a and -b */
};

static struct argp_option options[] = {
		// longname, shortname, arg, idk, help
		{"verbose", 'v', 0, 0, "Produce verbose output"},
		{"output", 'o', "OUTFILE", 0, "Output to OUTFILE instead of to standard output"},
		{0}
};


static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	struct arguments *arguments = state->input;

	switch (key)
	{
	case 'v':
		arguments->verbose = true;
		break;

	case 'o':
		arguments->outfile = arg;
		break;

	case ARGP_KEY_ARG:
	{
		if (state->arg_num >= ARGS_COUNT)
		{
			argp_usage(state);
		}
		arguments->args[state->arg_num] = arg;
		break;
	}
	case ARGP_KEY_END:
	{
		if (state->arg_num < ARGS_COUNT)
		{
			argp_usage(state);
		}
		break;
	}
	default: return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static char args_doc[] = "FILE";
static char doc[] = COMP_DOC;
static struct argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char **argv)
{
	struct arguments arguments;

	/* Set argument defaults */
	arguments.outfile = NULL;
	arguments.verbose = false;

	/* Where the magic happens */
	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	// if no output file is given the output file is input file 
	// but with the extension ".8px"
	if (arguments.outfile == NULL)
	{
		char *dot, *newname;
		int len;

		dot = strrchr(arguments.args[0], '.');
		if (dot) len = dot - arguments.args[0];
		else len = strlen(arguments.args[0]);
		
		newname = malloc(len + 6);
		
		strncpy(newname, arguments.args[0], len);
		strcpy(newname + len, "." BINARY_EXTENSION);

		arguments.outfile = newname;
	}

	char *src = readFile(arguments.args[0]);
	Chunk chunk;
	int state = compileFromSrc(src, &chunk);

	return state;
}