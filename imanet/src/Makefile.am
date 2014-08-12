imanetnetprogdir = bin/

bin_PROGRAMS = imanet_snd imanet_rcv

AM_CFLAGS = -Wall -pthread -lpcap

SRC_mtqueue = mtqueue.c \
	      mtqueue.h

SRC_imanet = imanet.c \
		 imanet.h

SRC_psniffer = psniffer.c \
	      psniffer.h

SRC_error_helper = error_helper.c \
		error_helper.h

imanet_snd_SOURCES = \
	$(SRC_mtqueue) \
	$(SRC_imanet) \
	$(SRC_psniffer) \
	$(SRC_error_helper) \
	main_snd.c

imanet_rcv_SOURCES = \
	$(SRC_mtqueue) \
	$(SRC_imanet) \
	$(SRC_psniffer) \
	$(SRC_error_helper) \
	main_rcv.c

