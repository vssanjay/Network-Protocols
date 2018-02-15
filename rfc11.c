#include <sys/time.h>

/**
*
*/
struct rfc11Protocol {
	unsigned short int messageLength;
	unsigned long int sequence; // int is only guaranteed to be 16-bits wide.
	long long timestamp; // long can be 4 bytes or 8 depending on the platform (only guaranteed to be minimum of 32 bits).  so, using long long to be safe.
	char *message; /* up to 1024 bytes in length, including the terminating null byte 0x00*/
};

struct rfc11Protocol parseMessage(char *bytes, int len) {

}

long long now() {
	
	struct timeval tv;
	gettimeofday(&tv, NULL);

	long long millisecondsSinceEpoch = (tv.tv_sec) * 1000LL + (tv.tv_usec) / 1000;
	return millisecondsSinceEpoch;
}

int buildMessage(char *msg, long sequenceNum, char **outBufferPtr) {
	struct rfc11Protocol m;
	int msgLen = strlen(msg);

	m.messageLength = 2 + 4 + 8 + msgLen;
	m.sequence = sequenceNum;
	m.timestamp = now();
	m.message = msg;

	*outBufferPtr = (char *) malloc(m.messageLength);
	*((unsigned short int*) *outBufferPtr) = htons(m.messageLength);
	*((unsigned long int*) (*outBufferPtr + 2)) = htonl(m.sequence);
	// timestamp has to be broken into two parts.  since htonl only works up until 4 bytes
	*((long*) (*outBufferPtr + 2 + 4)) = htonl((m.timestamp >> 32L));
	*((long*) (*outBufferPtr + 2 + 4 + 4)) = htonl(m.timestamp);// & 0xFFFFFFFFL);

	//hexDump("timestamp", &m.timestamp, sizeof(m.timestamp));
	memcpy((*outBufferPtr + 2 + 4 + 8), msg, msgLen);

	return m.messageLength;
}
