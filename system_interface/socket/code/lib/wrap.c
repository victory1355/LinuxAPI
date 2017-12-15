#include "head4sock.h"

ssize_t Write(int fildes, const void *buf, size_t nbyte)
{
	ssize_t retval = write(fildes, buf, nbyte);
	if(retval == -1)
	{
		perror("write() error");
		exit(0);
	}

	return retval;
}

ssize_t Read(int fildes, void *buf, size_t nbyte)
{
	int n;
	while((n=read(fildes, buf, nbyte)) == -1 && errno == EINTR);

	if(n == -1)
	{
		perror("read() failed");
		exit(0);
	}

	return n;
}

int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int n = bind(sockfd, addr, addrlen);
	if(n == -1)
	{
		perror("bind() failed");
		exit(0);
	}

	return n;
}

int Listen(int sockfd, int backlog)
{
	int n = listen(sockfd, backlog);
	if(n == -1)
	{
		perror("listen() failed");
		exit(0);
	}

	return n;
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
	int connfd = accept(sockfd, addr, addrlen);
	if(connfd == -1)
	{
		perror("accept() failed");
		exit(0);
	}

	return connfd;
}

int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	int n = connect(sockfd, addr, addrlen);
	if(n == -1)
	{
		perror("connect() failed");
		exit(0);
	}

	return n;
}

int Socket(int domain, int type, int protocol)
{
	int sockfd = socket(domain, type, protocol);
	if(sockfd == -1)
	{
		perror("socket() error");
		exit(0);
	}

	return sockfd;
}

int Setsockopt(int sockfd, int level, int optname,
	       const void *optval, socklen_t optlen)
{
	int retval = setsockopt(sockfd, level, optname, optval, optlen);
	if(retval == -1)
	{
		perror("setsockopt() error");
	}

	return retval;
}

int Select(int nfds, fd_set *readfds, fd_set *writefds,
	   fd_set *exceptfds, struct timeval *timeout)
{
	return select(nfds, readfds, writefds, exceptfds, timeout);
}
