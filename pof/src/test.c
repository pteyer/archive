#include "headers.h"
#include "pof.h"

int main(int argc, char* argv[])
{
	error_helper_init("/tmp/pof_test.log", LOG_LEVEL_DEBUG);
	const int max = 100000;
	int a = max;
	srand(time(NULL));
	int sd = pof_connect_to("localhost", "/tmp/pof", 10);
	fprintf(stderr, "connected on socket: %d\n", sd);
	while (a--) {
		struct pof_packet p;
		p.action = ACTION_PUT;
		int r = rand() % 1000000;
		sprintf(p.lot, "LOT_%d", a);
		sprintf(p.key, "KEY_%d", r);
		sprintf(p.value, "VALUE_%d_%d", r, r * 2);
		pof_send_to(sd, &p, sizeof(p), 0);

		p.action = ACTION_GET;
		r = rand();
		sprintf(p.lot, "LOT_%d", a);
		sprintf(p.key, "KEY_%d", r);
		pof_send_to(sd, &p, sizeof(p), 0);

		char buf[1024];
		pof_recv_from(sd, &buf, sizeof(buf), 0);
		sprintf(p.value, "VALUE_%d_%d", r, 2 * r);
		if (strcmp(buf, p.value) && strcmp(buf, "NULL")) {
			fprintf(stderr, "Error: sent->%s recvd->%s\n", p.value, buf);
		}
	}
	close(sd);
	return EXIT_SUCCESS;
}

