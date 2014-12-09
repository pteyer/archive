#ifndef POF_SOCKETS_H
#define POF_SOCKETS_H

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
int pof_connect_to(const char *host, const char *path, unsigned int time_out);

/**
 * Create a new socket, bind it to all incomming connections listen on the given
 * port number.
 * 
 * @param port Port number to listen to
 * @return File descriptor of the socket that is listening on success, -1 on 
 * failure
 */
int pof_listen_on(const char *path);

/**
 * Given a file descriptor that is already in listening state, try to accept
 * a new incomming connection and return the file descriptor.
 *
 * @param srv_fd File descriptor of a socket in listening mode
 * @return File descriptor of the new connection on success, -1 on failure
 */
int pof_accept_on(int socketfd);

/**
 * Given a file descriptor that is already in listening state, try to accept
 * a new incomming connection and return the file descriptor and IP address of
 * the client.
 *
 * @param srv_fd File descriptor of a socket in listening mode
 * @param ip A buffer to be filled by the IP address of the client
 * @return File descriptor of the new connection on success, -1 on failure
 */
int pof_accept_on_ip(int socketfd, char *ip_buf);

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
int pof_recv_from(int fd, void *buf, size_t nbytes, int flags);

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
int pof_send_to(int fd, const void *buf, size_t nbytes, int flags);

#endif

