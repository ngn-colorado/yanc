/**
 * yanc: Yet Another Network Controller
 *
 * Copyright 2013 Matt Monaco <matthew.monaco@colorado.edu>
 *
 * License TBD
 *
**/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <getopt.h>
#include <signal.h>
#include <pthread.h>
#include <fcntl.h>
#include <libgen.h>
#include <sys/stat.h>
#include "fs/schemafs.h"
#include "util/util.h"
#include "util/log.h"

struct y_opts {
	bool   daemonize;
	bool   multi_threaded;
	int    verbosity;
	char*  source;
	char*  mountpoint;
	char   options[1024];
	char*  schema_file;
};

static const struct y_opts Y_OPTS_DEFAULT = {
	.daemonize      = true,
	.multi_threaded = false,
	.verbosity      = LOG_DEBUG,
	.source         = "yanc",
	.mountpoint     = NULL,
	.options        = "auto_unmount",
	.schema_file    = "schema.txt",
};

void usage(int ret, const char* fmt, ...)
{
	FILE* out = (ret == 0) ? stdout : stderr;
	
	if (fmt != NULL) {
		va_list ap;
		va_start(ap, fmt);
		vfprintf(out, fmt, ap);
		va_end(ap);
		fputs("\n\n", out);
	}

	fprintf(out,
	"usage: yanc [options] [source] <mountpoint>\n"
	"\n"
	"  Mount the yanc file system on <mountpoint>. [source] is arbitrary and\n"
	"  and may be omitted.\n"
	"\n"
	"  options:\n"
	"\n"
	"    -S <file>           schema file, default: %s\n"
	"    -o <options>        comma-separated list of mount options\n"
	"                        default: %s\n"
	"    -f                  run in foreground\n"
	"    -s                  run fuse in single-threaded mode\n"
	"    -v                  increase verbosity\n"
	"    -q                  decrease verbosity\n"
	"    -h, --help          print this message and exit\n"
	"\n",
	Y_OPTS_DEFAULT.schema_file, Y_OPTS_DEFAULT.options
	);
	exit(ret);
}

static void y_parse_cmdline(int argc, char* argv[], struct y_opts* opts)
{
	int opt;

	static const struct option lopts[] = {
		{"help",   0,  NULL, 'h' },
		{"usage",  0,  NULL, 'h' },
		{ NULL,    0,  NULL,  0  }
	};

	while ((opt = getopt_long(argc, argv, "fo:S:svqh", lopts, NULL)) != -1) {

		switch (opt) {
		case 'S':
			opts->schema_file = optarg;
			break;
		case 'f':
			opts->daemonize = false;
			break;
		case 's':
			opts->multi_threaded = false;
			break;
		case 'v':
			if (opts->verbosity < LOG_DEBUG)
				opts->verbosity++;
			break;
		case 'q':
			if (opts->verbosity > LOG_EMERG)
				opts->verbosity--;
			break;
		case 'o':
			strncat(opts->options, ",", sizeof(opts->options));
			strncat(opts->options, optarg, sizeof(opts->options));
			break;
		case 'h':
			usage(EXIT_SUCCESS, NULL);
			break;
		case '?':
			usage(EXIT_FAILURE, "invalid option '%c'", optopt);
			break;
		default:
			fprintf(stderr, "internal getopt error 0%o\n", opt);
			exit(EXIT_FAILURE);
		}
	}

	if ((argc - optind) == 1) {
		opts->mountpoint = argv[optind];
	} else if ((argc - optind) == 2) {
		opts->source     = basename(argv[optind]);
		opts->mountpoint = argv[optind+1];
	} else {
		usage(EXIT_FAILURE, NULL);
	}
}

static void y_daemonize()
{
	pid_t pid, sid;

	pid = fork();

	if (pid < 0) {
		fprintf(stderr, "fork() error: %m");
		exit(EXIT_FAILURE);
	} else if (pid > 0) {
		_exit(EXIT_SUCCESS);
	}

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	umask(0);
	if (chdir("/") < 0) {
		fprintf(stderr, "chdir() error: %m");
		exit(EXIT_FAILURE);
	}

	sid = setsid();
	if (sid < 0) {
		fprintf(stderr, "setsid() error: %m");
		exit(EXIT_FAILURE);
	}
}

static void y_sig_handler(int sig)
{
	log_notice("caught signal: %s", strsignal(sig));
	exit(EXIT_SUCCESS);
}

static void y_setup_signals()
{
	if (signal(SIGINT, y_sig_handler) == SIG_ERR)
		log_warn("signal(SIGINT) error: %m");

	if (signal(SIGTERM, y_sig_handler) == SIG_ERR)
		log_warn("signal(SIGTERM) error: %m");
}

int main(int argc, char* argv[])
{
	struct y_opts opts = Y_OPTS_DEFAULT;

	// Generic daemon initialization
	
	y_parse_cmdline(argc, argv, &opts);
	log_init(opts.verbosity);
	if (opts.daemonize) y_daemonize();
	y_setup_signals();
	breakup_init();

	// Setup yanc

	log_info("Yanc v0.0");
	Schema* root = schema_fparse(opts.schema_file);

	// Start fuse (main_loop)

	return schemafs_run(argc, argv, root, opts.source, opts.mountpoint,
		opts.options, opts.multi_threaded);
}

