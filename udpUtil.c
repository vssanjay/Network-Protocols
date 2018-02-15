#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>


struct addrinfo * u_getServerInfo(char *hostName, unsigned short int port) {
	struct addrinfo hints, *serverInfo;

	// convert port from integer to string safely... port can not be more than 5 characters
	int portStrLen = sizeof(char) * 6;
	char *str_port = malloc(portStrLen);
	snprintf(str_port, portStrLen, "%d", port);

	// prepare the hints structure for getaddrinfo() call.  copied from example in slide
	memset(&hints, 0, sizeof hints); // zero out garbage values
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	int isError = getaddrinfo(hostName, str_port, &hints, &serverInfo);
	if (isError)
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(isError));

	return serverInfo;
}


/**
 * Listen on the specified port, buffering up to listenq connections.
 * @return The socket descriptor.
 */
 int u_bindUdp(struct addrinfo *serverInfo) {
	int sockFd;
	struct addrinfo *ptr;

	/* three steps...
	 *     1. Get socket descriptor
	 *     2. Bind on address and port
	 */
	for (ptr = serverInfo; ptr != NULL; ptr = ptr->ai_next) {
		// try to get socket
		sockFd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (sockFd == -1)
			continue;

		// clean up from last invocation.
		int yes = 1;
		setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);

		// try to bind
		if (bind(sockFd, ptr->ai_addr, ptr->ai_addrlen) == -1) {
			close(sockFd);
			continue;
		}
		break;
	}
	freeaddrinfo(serverInfo);
	if (ptr == NULL) {
		fprintf(stderr, "Server failed to bind to port %d", serverInfo->ai_protocol);
		return -1;
	}

	return sockFd;
}

int u_clientSocketUdp(struct addrinfo *serverInfo, struct addrinfo ** remoteAddrPtr) {
	int sockFd;
	struct addrinfo *ptr;

	/* three steps...
	 *     1. Get socket descriptor
	 *     2. Bind on address and port
	 */
	for (ptr = serverInfo; ptr != NULL; ptr = ptr->ai_next) {
		// try to get socket
		sockFd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (sockFd == -1)
			continue;

		break;
	}

	if (ptr == NULL) {
		fprintf(stderr, "Server failed to bind to port %d", serverInfo->ai_protocol);
		return -1;
	}

	*remoteAddrPtr = ptr;
	return sockFd;
}
