#include "headers.h"
#include "pof_sockets.h"

#define SIGFS_MAX_PORT_LEN 10
#define SIGFS_MAX_BACKLOG SOMAXCONN

/**
 * Given a host name and a port number, this function tries to connect to the 
 * host within the given amount of time out.
 *
 * @param host Hostname of the machine to connect to
 * @param port Port number that the host machine is listening to
 * @param time_out Amount of time in seconds that it tries to connect before 
 * reporting a failure
 * @return File descriptor of the connection in success, -1 on failure
 */
int pof_connect_to(const char *host, const char *path, unsigned int time_out)
{
	int fd;
	struct sockaddr_un sa;

	err_fatal_neg1(fd = socket(AF_UNIX, SOCK_STREAM, 0), 
			return -1);

	sa.sun_family = AF_UNIX;
	strcpy(sa.sun_path, path);

	int flag;
	do {
		flag = 0;
		err_fatal_neg1(connect(fd, (struct sockaddr *)&sa, sizeof(sa)), 
				flag = 1);
		if (flag) {
			if (!time_out)
				return -1;
			sleep(1);
			time_out--;
		}
	} while (flag);

	return fd;
}

/**
 * Create a new socket, bind it to all incomming connections listen on the given
 * port number.
 * 
 * @param port Port number to listen to
 * @return File descriptor of the socket that is listening on success, -1 on 
 * failure
 */
int pof_listen_on(const char *path)
{
	int fd;
	struct sockaddr_un sa;
	int yes;

	sa.sun_family = AF_UNIX;
	strcpy(sa.sun_path, path);

	err_fatal_neg1(fd = socket(AF_UNIX, SOCK_STREAM, 0), 
			return -1);

	yes = 1;
	err_fatal_nzero(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, 
				sizeof(yes)), 
			return -1);

	err_fatal_nzero(bind(fd, (struct sockaddr *)&sa, sizeof(sa)), 
			return -1);

	err_fatal_nzero(listen(fd, SIGFS_MAX_BACKLOG), 
			return -1);

	return fd;
}

/**
 * Given a file descriptor that is already in listening state, try to accept
 * a new incomming connection and return the file descriptor.
 *
 * @param srv_fd File descriptor of a socket in listening mode
 * @return File descriptor of the new connection on success, -1 on failure
 */
int pof_accept_on(int srv_fd)
{
	struct sockaddr_un sa;
	int clnt_fd;
	socklen_t slen = sizeof(sa);

	clnt_fd = accept(srv_fd, (struct sockaddr *)&sa, &slen);
	return clnt_fd;
}

#define __BUFSIZE_VALIDATE_FLAG 0x9381028

/**
 * During every send and recieve function, first we send a packet containing the 
 * size of the next packet and a flag which is used for vaildation. This packet
 * is of type bufsize_t.
 */
struct bufsize_t {
	int size;
	int flag;
};

/**
 * This function tries to recieve nbytes of data from client connected to the 
 * local machine through file descriptor fd, using the given flags.
 * Note that here we will valiate the incomming size and ignore the EAGAIN error.
 *
 * @param fd File descriptor of the socket to read from
 * @param buf A buffer to be filled by data
 * @param nbytes Number of bytes to be read from socket
 * @param flags Flags to pas to recv system call.
 * @return Number of bytes read from socket
 */
int pof_recv_from(int fd, void *buf, size_t nbytes, int flags)
{
	int nread = 0, c;
	struct bufsize_t bs;

	err_fatal_neg1(c = recv(fd, &bs, sizeof (bs), 0), 
			return -1);

	if (bs.flag == __BUFSIZE_VALIDATE_FLAG) {
		nbytes = nbytes < bs.size ? nbytes : bs.size;
	} else 
		return 0;

	while (nbytes) {
		c = recv(fd, buf + nread, nbytes, flags);
		if (-1 == c) {
			if ((EINTR == errno) || (EAGAIN == errno))
				continue;
			else 
				return nread;
		}
		if (0 == c) 
			return nread;

		nbytes -= c;
		nread += c;
	}

	return nread;
}

/**
 * A function to make sure the given buffer is completely sent to the given
 * file descriptor.
 *
 * @param fd is the file descriptor of the socket
 * @param buf buffer to send
 * @param nbytes length of the buffer
 * @param flags the flags to be sent to send system call
 * @return Number of bytes sent
 */
int pof_send_to(int fd, const void *buf, size_t nbytes, int flags)
{
	int nsent = 0, c;
	struct bufsize_t bs = {
		.flag = __BUFSIZE_VALIDATE_FLAG,
		.size = nbytes
	};

	err_fatal_neg1(c = send(fd, &bs, sizeof (bs), 0),
			return -1);

	while (nbytes > 0) {
		c = send(fd, buf + nsent, nbytes, flags);

		if (-1 == c) {
			if(EINTR == errno) 
				continue;
			else 
				return nsent;
		}

		if (0 == c) {
			return nsent;
		}

		nbytes -= c;
		nsent += c;
	}

	return nsent;
}
