#include "headers.h"
#include "pof.h"
#include "pof_store.h"

#define MAX_HASH_STORES 1024

static int server_socket = 0;

static void init(void)
{
	void *p = malloc(1024 * 1024 * 1024);
	memset(p, 0, 1024 * 1024);
	if (p) free(p);
	error_helper_init("/tmp/pof.log", LOG_LEVEL_FATAL);
	pof_store_init();
	err_fatal_neg1(server_socket = pof_listen_on("/tmp/pof"), 
			goto failure);
	return;
failure:
	fprintf(stderr, "Cannot listen on /tmp/pof\n");
	exit(EXIT_FAILURE);
}

static void event_loop(void)
{
	struct pof_packet buf;
	while (1) {
		memset(&buf, 0, sizeof(buf));
		int remote = pof_accept_on(server_socket);
		int count = 0;
		while(pof_recv_from(remote, &buf, sizeof(buf), 0)) {
			if (buf.action == ACTION_PUT) {
				count++;
				err_warn_zero(put(buf.lot, buf.key, buf.value, strlen(buf.value) + 1));
			} else if (buf.action == ACTION_GET) {
				count++;
				struct pof_store_entry *e = get(buf.lot, buf.key);
				if (e) {
					pof_send_to(remote, e->data, e->len, 0);
				} else {
					pof_send_to(remote, "NULL", 5, 0);
				}
			}
			if (! (count % 10000)) {
				fprintf(stderr, "%d\n", count);
			}
		}
		close(remote);
	}
}

int main(int argc, char* argv[])
{
	init();
	event_loop();
	pof_store_destroy();
	return EXIT_SUCCESS;
}

