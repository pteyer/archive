#ifndef POF_STORE_H
#define POF_STORE_H

#define ACTION_PUT 'P'
#define ACTION_GET 'G'

#define POF_STORE_MAX_SIZE 100 * 1000 * 1000

#define MAX_LEN_HASH 100000
#define MAX_LEN_KEY 128
#define MAX_LEN_NAME 128
#define MAX_LEN_VALUE 1024

struct pof_store_entry {
	size_t len;
	void *data;
};

int pof_store_init(void);
struct pof_store_entry *get(char *lot, char *key);
int put(char *lot, char *key, void *value, size_t len);

#endif

