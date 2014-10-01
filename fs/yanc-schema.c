#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "schema.h"

void usage(const char* arg0)
{
	fprintf(stderr,
	"usage: %s [options] [input-file]\n"
	"\n"
	"  This small utility takes a Yanc schema as plain text and outputs\n"
	"  it in a binary form.\n"
	"\n"
	"  options:\n"
	"    -o file        output to file rather than stdout\n"
	"    -t             test: parse, convert to binary, convert back to text, and print\n"
	"    -h             print this message an exit\n"
	"\n",
	arg0
	);
}

int main(int argc, char* argv[])
{
	int opt;
	const char* infilename  = NULL;
	const char* outfilename = NULL;
	int test = 0;
	FILE*   out;

	while ((opt = getopt(argc, argv, "o:th")) != -1) {

		switch (opt) {
		case 'o':
			outfilename = optarg;
			break;
		case 't':
			test = 1;
			break;
		case 'h':
			usage(argv[0]);
			return EXIT_SUCCESS;
			break;
		default:
			usage(argv[0]);
			return EXIT_FAILURE;
		}
	}

	if (argc - optind > 1) {
		usage(argv[0]);
		return EXIT_FAILURE;
	} else if (argc - optind == 1) {
		infilename = argv[optind];
	}

	if (outfilename)
		out = fopen(outfilename, "w");
	else
		out = stdout;

	if (!out) {
		fprintf(stderr, "ERROR unable to open %s: %m\n", outfilename);
		return EXIT_FAILURE;
	}

	////////////////////////////////////////////////////////////////////////

	Schema* s = NULL;
	char*   b;
	char*   end;

	s = schema_fparse(infilename);
	b = schema_pack(s, &end);
	schema_free(s);

	if (test) {
		s = schema_unpack(b);
		schema_printf(s, out);
		schema_free(s);
	} else {
		fwrite(b, end-b, 1, out);
	}

	free(b);
	fclose(out);

	return EXIT_SUCCESS;
}

