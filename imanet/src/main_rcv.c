#include "headers.h"
#include "imanet.h"
#include "psniffer.h"
#include "mtqueue.h"

#define ARGC_MIN 	3
#define ARGC_MAX 	3

// interface name to sniff on
static char *interface = NULL;

// show usage message
static void show_usage(const char *prgname)
{ fprintf(stderr, "Usage: %s -i <interface>\n", prgname); }

// check the command line arguments for initity
// returns 0 on success and -1 on success
static int init_args(int argc, char *argv[])
{
	// check arguments first, if not init then print usage and exit with
	// error
	if (argc > ARGC_MAX || argc < ARGC_MIN) {
		// invalid number of arguments
		return -1;
	}

	char c;
	while (-1 != (c = getopt(argc, argv, "i:"))) {
		switch (c) {
			case 'i':
				interface = malloc(strlen(optarg) + 1);
				strcpy(interface, optarg);
				break;
			default:
				// invalid argument
				return -1;
		}
	}

	return 0;
}

// release all alocated memory
static void destroy_all(void)
{
	if (interface) {
		free(interface);
	}
}

static void exit_failure(void)
{
	destroy_all();
	exit(EXIT_FAILURE);
}

static void log_cmd_args(const int argc, char *argv[])
{
	// log the command line arguments
	char cmds[MAX_LOG_LEN] = "command line arguments: ";
	int i;
	for (i = 0; i < argc; ++i) {
		strcat(cmds, argv[i]);
		strcat(cmds, " ");
	}
	log_info(cmds);
}

// starting point of the program
int main(int argc, char *argv[])
{
	if (init_args(argc, argv)) {
		const char *prgname = argv[0];
		show_usage(prgname);

		exit_failure();
	}

	struct imanet_handler ih;
	err_fatal_nzero(imanet_init(&ih, interface),
		exit_failure());

	log_cmd_args(argc, argv);

	while (1) {
		struct imanet_pkt p;
		int l = imanet_recv(&ih, &p);
		if (l < 0) {
			fprintf(stderr, "Something went wrong with the receiver\n");
		} else {
			fprintf(stderr, "TLEN: %d DLEN: %d PACKET: \'%s\'\n", l,
					p.len, p.data);
		}
	}

	pthread_join(ih.recv_thrd, NULL);

	return EXIT_SUCCESS;
}

