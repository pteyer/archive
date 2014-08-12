#ifndef MTQUEUE_H
#define MTQUEUE_H

#include <pthread.h>

// to make sure of thread safety in C functions
#ifndef _REENTRANT
# define _REENTRANT
#endif

struct mtqueue_node {
	struct mtqueue_node *next;
	struct mtqueue_node *prev;

	void *data;
	size_t len;
};

struct mtqueue {
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	struct mtqueue_node *front;
	struct mtqueue_node *back;
};

void mtq_init(struct mtqueue *q);
void mtq_destroy(struct mtqueue *q);

void mtq_wait(struct mtqueue *q);
void mtq_signal(struct mtqueue *q);

int mtq_push_back(struct mtqueue *q, void *dp, size_t len);
void *mtq_pop_front(struct mtqueue *q, size_t *len);

size_t mtq_get_size(const struct mtqueue *q);
int mtq_is_empty(const struct mtqueue *q);

#endif

