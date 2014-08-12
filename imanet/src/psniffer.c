#include "headers.h"
#include "psniffer.h"
#include "mtqueue.h"

extern struct ether_header IMANET_PKT_ETH_HEADER;

static int pkt_valid(void *p, const struct pcap_pkthdr *ph)
{
	struct imanet_pkt *ip = (struct imanet_pkt *)p;
	return (! memcmp(&ip->ethh, &IMANET_PKT_ETH_HEADER, sizeof(ip->ethh))
			&& ph->len == IMANET_PKT_LEN);
}

static void newpkt_callback(u_char *param, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
	struct imanet_handler *ih = (struct imanet_handler *)param;

	if (! pkt_valid((void *)packet, pkthdr)) {
		log_debug("non-imanet packet received and dropped");
		return;
	}

	log_debug("pushing imanet packet into the receiver queue");

	struct imanet_pkt *p;
	err_fatal_null(p = malloc(sizeof(struct imanet_pkt)),
		      return);
	memcpy(p, packet, pkthdr->len);
	mtq_push_back(&ih->bufq, p, pkthdr->len);

	if (pkthdr->len != pkthdr->caplen) {
		log_warn("length of the captured packet is not equal to the total length of packet");
	}

	if (! mtq_is_empty(&ih->bufq)) {
		mtq_signal(&ih->bufq);
	}

	return;
}

static void *run_sniffer(void *param)
{
	struct imanet_handler *ih = (struct imanet_handler *)param;
	while (1) {
		if (pcap_loop(ih->pcaph, -1, newpkt_callback, (u_char *)ih) <= 0) {
			log_warn("live packet capture function returned which should not return");
			log_fatal(pcap_geterr(ih->pcaph));
		}
	}
	return NULL;
}

static pthread_t start_thread(struct imanet_handler *ih)
{ 
	pthread_t t;
	pthread_create(&t, NULL, run_sniffer, ih); 
	return t;
}

pthread_t psniffer_start_thread(struct imanet_handler *ih)
{ return start_thread(ih); }

