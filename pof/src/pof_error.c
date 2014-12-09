#include "headers.h"
#include "pof_error.h"

static FILE *logfp = NULL;
static enum log_level llev = -1;
static pid_t pid;
static pthread_mutex_t logfmutex;

void error_helper_init(const char *logfpath, enum log_level lev)
{
	// check if init is called before
	if (logfp) {
		fprintf(stderr, "Log file already initialized");
		return;
	}

	// try to open log file
	if (! (logfp = fopen(logfpath, "a"))) {
		perror("Opening log file");
		fprintf(stderr, "Setting log file pointer to stderr\n");
		logfp = stderr;
	} else {
		fprintf(stderr, "Log file initialized successfully and can be found at '%s'\n", logfpath);
	}

	// init pid
	pid = getpid();

	// initializing log file mutex
	pthread_mutex_init(&logfmutex, NULL);

	// setting base log level
	llev = lev;
}

void error_helper_destroy(void)
{
	llev = -1;
	if (logfp && logfp != stderr) {
		fflush(logfp);
		fclose(logfp);
	}
	pthread_mutex_destroy(&logfmutex);
}

enum log_level get_log_level(void)
{
	return llev;
}

// return 0 if successful, and -1 otherwise
static int do_add_log(const char *msg, enum log_level lev)
{
	if (lev < get_log_level()) {
		return 0;
	}

	int llen = 0;
	int nwrote = 0;
	
	pthread_mutex_lock(&logfmutex);
	llen = strlen(msg);
	nwrote = fprintf(logfp, "{%d-%lu} %s", pid, pthread_self(), msg);
	fflush(logfp);
	pthread_mutex_unlock(&logfmutex);

	if (nwrote < llen) {
		return -1;
	}

	return 0;
}

static int is_logfp_valid(void)
{ return (logfp != NULL); }

void do_log_mute(const char *msg, enum log_level lev)
{
	if (! is_logfp_valid()) {
		logfp = stderr;
		fprintf(stderr, "Error helper module not initalized properly\n");
	}
	if (do_add_log(msg, lev)) {
		fprintf(stderr, "Problem in logging message: %s", msg);
	}
}

