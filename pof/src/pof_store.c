#include "headers.h"
#include "pof_store.h"

int pof_store_init(void)
{
	return hcreate(POF_STORE_MAX_SIZE);
}

static ENTRY *do_get(char *lot, char *key)
{
	(void) lot;
	ENTRY e;
	e.key = key;
	ENTRY *ep = hsearch(e, FIND);
	return ep ? ep : NULL;
}

struct pof_store_entry *get(char *lot, char *key) 
{
	ENTRY *p = do_get(lot, key);
	if (!p) {
		return NULL;
	}
	return (struct pof_store_entry *)p->data;
}

static int do_put(char *lot, char *key, void *value)
{
	(void) lot;
	ENTRY e;
	e.key = key;
	e.data = value;
	return !hsearch(e, ENTER);
}

int put(char *lot, char *key, void *value, size_t len)
{
	(void) lot;

	ENTRY *oe;
	if ((oe = do_get(lot, key))) {
		free(oe->key);
		free(((struct pof_store_entry *)oe->data)->data);
		free(oe->data);
	}

	struct pof_store_entry *e;
	err_fatal_null(e = malloc(sizeof(*e)), return -1);

	void *d; 
	err_fatal_null(d = malloc(len), return -1);
	memcpy(d, value, len);

	char *k;
	err_fatal_null(k = malloc(strlen(key) + 1), return -1);
	strcpy(k, key);

	e->data = d;
	e->len = len;

	return do_put(lot, k, e);
}

static void do_destroy_hash(void)
{
	hdestroy();
}

void pof_store_destroy(void)
{
	do_destroy_hash();
}

