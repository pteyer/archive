#ifndef POF_H
#define POF_H

#define ACTION_PUT 'P'
#define ACTION_GET 'G'

#define MAX_LEN_HASH 100000
#define MAX_LEN_KEY 128
#define MAX_LEN_NAME 128
#define MAX_LEN_VALUE 1024

struct pof_packet {
	char action;
	char lot[MAX_LEN_NAME];
	char key[MAX_LEN_KEY];
	char value[MAX_LEN_VALUE];
};

#endif

