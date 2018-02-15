#include <sys/time.h>

/**
*
*/
struct rfc12ProtocolRequest {
	unsigned long int a;
	unsigned long int b; // int is only guaranteed to be 16-bits wide.
	char *operand; /* up to 1024 bytes in length, including the terminating null byte 0x00*/
};

struct rfc12ProtocolResponse {
	unsigned long int a;
	unsigned long int b;
	unsigned long int answer;
	char *operation ;
	int isValid ;

}



int buildMessageRequest(char operation, unsigned long int a, unsigned long int b , char **outBufferPtr) {
	struct rfc12ProtocolRequest m;
	
	m.operation = operation;
	m.a = arg1;
	m.b = arg2;

	*outBufferPtr = (char *) malloc(9);
	*(*outBufferPtr) = m.operation;
	*((unsigned long int*) (*outBufferPtr + 1)) = htonl(m.arg1);
	//*((unsigned long int*) (*outBufferPtr + 1 + 4)) = htonl(m.arg2);
	int arg2_networkOperand = htonl(m.arg2);
	memcpy((*outBufferPtr + 1 + 4), &arg2_networkOperand, 4);

	return 9;
}

int buildServerResponse(char operation, unsigned long int a, unsigned long int b, unsigned long int answer, int isValid, char **outBufferPtr) {
	struct rfc12ProtocolResponse m;
	
	m.operation = operation;
	m.a = arg1;
	m.b = arg2;
	m.answer = result;
	m.isValid = isValid;

	*outBufferPtr = (char *) malloc(14);
	*(*outBufferPtr) = m.operation;
	*((unsigned long int*) (*outBufferPtr + 1)) = htonl(m.arg1);
	*((unsigned long int*) (*outBufferPtr + 1 + 4)) = htonl(m.arg2);
	*((unsigned long int*) (*outBufferPtr + 1 + 4 + 4)) = htonl(m.answer);
	*((char*) (*outBufferPtr + 1 + 4 + 4)) = htonl(m.isValid);

	

	//hexDump("timestamp", &m.timestamp, sizeof(m.timestamp));
	memcpy((*outBufferPtr + 1 + 4 + 4), msg, msgLen);

	return m.messageLength;
}
