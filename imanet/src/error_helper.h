#ifndef AUX_ERR_H
#define AUX_ERR_H

#include "headers.h"

#define MAX_CFG_LINE_LEN 	1024
#define MAX_LOG_LEN 		1024

enum log_level {
	LOG_LEVEL_DEBUG = 0,
	LOG_LEVEL_INFO = 1,
	LOG_LEVEL_WARN = 2,
	LOG_LEVEL_ERROR = 3,
	LOG_LEVEL_FATAL = 4,
	LOG_LEVEL_OFF = 5
};

void error_helper_init(const char *logfpath, enum log_level lev);
void error_helper_destroy(void);
void do_log_mute(const char *msg, enum log_level lev);
enum log_level get_log_level(void);

#define do_log(msg, lev, levstr) \
do { \
	char str[MAX_LOG_LEN]; \
	time_t now; \
	time(&now); \
	snprintf(str, MAX_LOG_LEN, "%s %s * %s:%d[%s] - %s\n\n", \
		levstr, \
		ctime(&now), \
		__FILE__, \
		__LINE__, \
		__func__, \
		msg \
	); \
	do_log_mute(str, lev); \
} while (0)

#define log_debug(msg) 		do_log(msg, LOG_LEVEL_DEBUG, "DEBUG")
#define log_info(msg) 		do_log(msg, LOG_LEVEL_INFO, "INFO")
#define log_warn(msg)	 	do_log(msg, LOG_LEVEL_WARN, "WARN")
#define log_fatal(msg) 		do_log(msg, LOG_LEVEL_FATAL, "FATAL")

// revised upto here

#define log_warn_strerror(cmd) \
do { \
	char msg[MAX_LOG_LEN]; \
	snprintf(msg, MAX_LOG_LEN, "%s: %s", #cmd, strerror(errno)); \
	log_warn(msg); \
} while (0)

#define log_fatal_strerror(cmd) \
do { \
	char msg[MAX_LOG_LEN]; \
	snprintf(msg, MAX_LOG_LEN, "%s: %s", #cmd, strerror(errno)); \
	log_fatal(msg); \
} while (0)

#define warn_log(cmd, val) \
do { \
	if ((val) == (cmd)) { \
		log_warn_strerror(cmd); \
	} \
} while (0)

#define warn_log_n(cmd, nval) \
do { \
	if ((nval) != (cmd)) { \
		log_warn_strerror(cmd); \
	} \
} while (0)

#define fatal_log(cmd, val, action) \
do { \
	if ((val) == (cmd)) { \
		log_fatal_strerror(cmd); \
		action; \
	} \
} while (0)

#define fatal_log_n(cmd, nval, action) \
do { \
	if ((nval) != (cmd)) { \
		log_fatal_strerror(cmd); \
		action; \
	} \
} while (0)

#define err_warn(cmd, val) 		warn_log(cmd, val)
#define err_warn_n(cmd, val) 		warn_log_n(cmd, val)
#define err_fatal(cmd, val, action) 	fatal_log(cmd, val, action)
#define err_fatal_n(cmd, val, action) 	fatal_log_n(cmd, val, action)

#define err_warn_neg1(cmd) 		err_warn(cmd, -1)
#define err_warn_nzero(cmd) 		err_warn_n(cmd, 0)
#define err_warn_zero(cmd) 		err_warn(cmd, 0)
#define err_warn_null(cmd) 		err_warn(cmd, NULL)
#define err_fatal_neg1(cmd, action) 	err_fatal(cmd, -1, action)
#define err_fatal_nzero(cmd, action) 	err_fatal_n(cmd, 0, action)
#define err_fatal_zero(cmd, action) 	err_fatal(cmd, 0, action)
#define err_fatal_null(cmd, action) 	err_fatal(cmd, NULL, action)

#endif

