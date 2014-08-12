#ifndef PSNIFFER_H
#define PSNIFFER_H

#define MAX_FACE_LEN 1024
#define MAX_PACKET_LEN 1500

#include <pthread.h>
#include "mtqueue.h"
#include "imanet.h"

pthread_t psniffer_start_thread(struct imanet_handler *ih);

#endif


