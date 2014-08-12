#include "headers.h"
#include "mtqueue.h"

// queue size
static volatile size_t q_sz = 0;

// lock queue mutex
static inline void lockq(struct mtqueue *q)
{ pthread_mutex_lock(&q->mutex); }

// unlock queue mutex
static inline void unlockq(struct mtqueue *q)
{ pthread_mutex_unlock(&q->mutex); }

// initialize the mutex and head
void mtq_init(struct mtqueue *q)
{
	pthread_mutex_init(&(q->mutex), NULL);
	pthread_cond_init(&(q->cond), NULL);
	q->front = NULL;
	q->back = NULL;
}

// go through all elements and free the allocated memory
void mtq_destroy(struct mtqueue *q)
{
	lockq(q);
	struct mtqueue_node *pin = q->front;
	while (pin) {
		struct mtqueue_node *next = pin->next;
		free(pin);
		q_sz--;
		pin = next;
	}
	q->front = q->back = NULL;
	unlockq(q);

	pthread_mutex_destroy(&(q->mutex));
}

// wait on the queue until signalled
void mtq_wait(struct mtqueue *q)
{
	lockq(q);
	pthread_cond_wait(&(q->cond), &(q->mutex));
	unlockq(q);
}

// signal whatever thread that is waiting on this queue
void mtq_signal(struct mtqueue *q)
{
	lockq(q);
	pthread_cond_signal(&(q->cond));
	unlockq(q);
}

// push data pointed by dp to the back of the queue
int mtq_push_back(struct mtqueue *q, void *dp, size_t len)
{
	struct mtqueue_node *n;
	err_fatal_null(n = malloc(sizeof (struct mtqueue_node)),
		return -1);
	
	n->data = dp;
	n->len = len;

	lockq(q);
	if (!(q->back)) {
		// empty queue
		q->back = n;
		q->back->next = q->back->prev = NULL;
		q->front = q->back;
	} else {
		n->next = NULL;
		n->prev = q->back;
		q->back->next = n;
		q->back = n;
	}
	q_sz++;
	unlockq(q);

	return 0;
}

// return NULL if queue is empty, otherwise a pointer to the data
void *mtq_pop_front(struct mtqueue *q, size_t *len)
{
	void *ret = NULL;

	lockq(q);
	if (!(q->front)) {
		// empty queue
		*len = 0;
	} else {

		// last element
		if (q->front == q->back) {
			q->back = NULL;
		}

		ret = q->front->data;
		*len = q->front->len;

		struct mtqueue_node *f = q->front;
		q->front = q->front->next;
		free(f);

		q_sz--;
	}
	unlockq(q);

	return ret;
}

// get queue size
size_t mtq_get_size(const struct mtqueue *q)
{ return q_sz; }

// check if queue is empty
int mtq_is_empty(const struct mtqueue *q)
{ return (mtq_get_size(q) == 0); }

