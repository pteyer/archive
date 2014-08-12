#include "imanet.h"
#include "psniffer.h"
#include "headers.h"

const struct ether_header IMANET_PKT_ETH_HEADER = {
	.ether_dhost = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
	.ether_shost = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01},
	.ether_type = 0x00
};

static void init_logger(void)
{
	error_helper_init(IMANET_LOG_FILE_PATH, LOG_LEVEL_DEBUG);
	log_info("initializing imanet network");
}

static int init_pcap(struct imanet_handler *ih)
{
	log_debug("trying to initialize sniffer module");
	
	char errbuf[PCAP_ERRBUF_SIZE] = "";
	err_fatal_null(ih->pcaph = pcap_open_live(ih->dev, BUFSIZ, 1, 0, errbuf),
			goto exit_failure);

	if (strlen(errbuf) > 0) {
		// this is for warning messages from pcap
		log_warn(errbuf);
	}
	return 0;

exit_failure:
	log_fatal(errbuf);
	return -1;
}

static void init_bufq(struct imanet_handler *ih)
{
	mtq_init(&ih->bufq);
}

static int init_receiver_thread(struct imanet_handler *ih)
{
	log_info("trying to start the packet sniffer thread...");
	err_fatal_zero(ih->recv_thrd = psniffer_start_thread(ih),
			return -1);
	return 0;
}

int imanet_init(struct imanet_handler *ih, const char *dev)
{
	init_logger();

	size_t devlen = strlen(dev) + 1;
	err_fatal_null(ih->dev = malloc(devlen),
			goto exit_failure);
	strcpy(ih->dev, dev);

	err_fatal_nzero(init_pcap(ih),
			goto exit_failure);

	init_bufq(ih);

	err_fatal_nzero(init_receiver_thread(ih),
			goto exit_failure);

	return 0;

exit_failure:
	imanet_destroy(ih);
	return -1;
}

int imanet_destroy(struct imanet_handler *ih)
{
	if (ih) {
		if (ih->dev) {
			free(ih->dev);
		}
	}
	return 0;
}

// TODO all packets must be of size ETH_FRAME_LEN
// if given size is more than that, divide packets to that size and send
// multiple packets.  if lower than that, pad to zero
struct imanet_pkt *make_packet(struct imanet_pkt *buf, const void *data, uint16_t size)
{
	memset(buf, 0, IMANET_PKT_LEN);
	buf->ethh = IMANET_PKT_ETH_HEADER;
	buf->len = size;
	memcpy(buf->data, data, size);

	return buf;
}

int imanet_send(struct imanet_handler *ih, const void *buf, size_t len)
{
	while (len) {
		uint16_t l = IMANET_PKT_DATA_LEN >= len ? len : IMANET_PKT_DATA_LEN;
		struct imanet_pkt p;
		make_packet(&p, buf, l);
		buf += l;
		len -= l;

		err_fatal_nzero(pcap_sendpacket(ih->pcaph, (const u_char *)&p, IMANET_PKT_LEN),
				goto exit_fail);
	}

	return 0;

exit_fail:
	log_warn(pcap_geterr(ih->pcaph));
	return -1;
}

int imanet_recv(struct imanet_handler *ih, struct imanet_pkt *buf)
{
	if (mtq_is_empty(&ih->bufq)) 
		mtq_wait(&ih->bufq);

	size_t l;
	void *bp = mtq_pop_front(&ih->bufq, &l);

	if (l != IMANET_PKT_LEN) {
		log_warn("length of packet read from queue is not equal to a standard packet size");
	}

	memcpy(buf, bp, l);
	free(bp);
	
	return 0;
}

