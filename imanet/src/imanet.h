#ifndef IMANET_H
#define IMANET_H

#include "headers.h"
#include "mtqueue.h"

#ifndef IMANET_LOG_FILE_PATH
# define IMANET_LOG_FILE_PATH 	"/var/log/imanet.log"
#endif

/** imanet packets length */
#define IMANET_PKT_LEN 		ETH_FRAME_LEN

/** imanet packets data length */
#define IMANET_PKT_DATA_LEN 	(ETH_FRAME_LEN - (sizeof(uint16_t) + sizeof(struct ether_header)))

/** imanet packets header length */
#define IMANET_PKT_HDR_LEN 	(IMANET_PKT_LEN - IMANET_PKT_DATA_LEN)

/** @struct imanet packet structure */
struct imanet_pkt {
	struct ether_header ethh;  /** ethernet header */
	uint16_t len;  /** length of data in packet */
	unsigned char data[IMANET_PKT_DATA_LEN];  /** data payload */
};

/** @struct imanet handler structure */
struct imanet_handler {
	pcap_t *pcaph;  /** pcap handler */
	struct mtqueue bufq;  /** multithreaded queue used to buffer reads */
	uint16_t port;  /** port number - not used yet */
	char *dev;  /** device to read/write from/to */
	pthread_t recv_thrd;  /** receiver thread handler */
};

/**
 * Function to initiate an imanet network. This function has to be called at the
 * beginning of the program to create the imanet network.
 * @param ih imanet handler to be initialized
 * @param dev device to read/write from/to
 * @return Returns zero on success and a non-zero value on failure
 * @see imanet_destroy()
 */
int imanet_init(struct imanet_handler *ih, const char *dev);

/**
 * Function to destroy the imanet handler. This function has to be called at the
 * end of the program.
 * @param ih imanet handler to be destroyed
 * @return Returns zero on sucess and a non-zero value on failure
 */
int imanet_destroy(struct imanet_handler *ih);

/** 
 * Function to broadcast the given buffer with the given length on the device
 * used to initialize the imanet handler. Note that if the buffer is larger than
 * one ethernet packet size, it will be divided into as many packets as
 * necessary, and if smaller than a ethernet packet size, it will be padded to
 * zeros.
 * @param ih imanet handler
 * @param buf buffer to send
 * @param len length of data in buffer
 * @return Returns zero on success and a non-zero value on failure
 */
int imanet_send(struct imanet_handler *ih, const void *buf, size_t len);

/**
 * Function to read one packet from the device used in initializing the imanet
 * handler.
 * @param ih imanet handler
 * @param buf imanet packet buffer to be filled with the data read from device
 * @return Returns zero on success and a non-zero value on failure
 */
int imanet_recv(struct imanet_handler *ih, struct imanet_pkt *buf);

#endif

