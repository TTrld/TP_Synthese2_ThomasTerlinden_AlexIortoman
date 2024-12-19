#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 17
#define BUF_ACKSIZE 4
#define LEN_DATA 512

int main(int argc, char *argv[]){
	
	printf("Client started.\n");
	
	const char *hostName = argv[1];
	const char *fileName = argv[2];
	const char *portNumber = argv[3];  //1069
	
	int clientSocket; 
	ssize_t sndto; 
	ssize_t rcvFrom;
	struct addrinfo hints; 
	struct addrinfo *results; 
	struct addrinfo *record;
	
	char buf_senddata[BUF_SIZE] = {};
	char buf_ackdata[BUF_ACKSIZE] = {};
		
	memset(&hints, 0, sizeof(struct addrinfo));
	
	hints.ai_family = AF_INET; 
	hints.ai_protocol = IPPROTO_UDP; 
	hints.ai_socktype = SOCK_DGRAM;
	
	if (getaddrinfo(hostName, portNumber, &hints, &results) != 0){
		perror("Getaddrinfo Failed.");
		exit(EXIT_FAILURE);
	}
	
	for (record = results; record != NULL; record = record->ai_next){
		clientSocket = socket(record->ai_family, record->ai_socktype, record->ai_protocol);
		if (clientSocket == -1) continue;
	}
	
	// buffer creation for sending message according to RFC 1350
	/*
	 2 bytes     string    1 byte    string    1 byte
	--------------------------------------------------
	| Opcode |  Filename  |   0  |    Mode    |   0  |
	--------------------------------------------------
	*/	
	char *mode = "octet";
	size_t nbchar = 0;
	buf_senddata[0] = 0; // Opcode 1/2
	buf_senddata[1] = 1; // Opcode 2/2 : Read request (RRQ)
	nbchar = nbchar + 2;
	for (int i = 0; i<strlen(fileName); i++){ // Filename
		buf_senddata[i+2] = fileName[i];
		nbchar++;
	}
	buf_senddata[2 + strlen(fileName)] = 0;
	nbchar++;
	for (int i = 0; i<strlen(mode); i++){ // Mode
		buf_senddata[2 + strlen(fileName) + 1 + i] = mode[i];
		nbchar++;
	}
	buf_senddata[3 + strlen(fileName) + strlen(mode)] = 0;
	nbchar++;

	// RRQ to server
	sndto = sendto(clientSocket, buf_senddata, nbchar, MSG_EOR, results->ai_addr, results->ai_addrlen);
	if (sndto < 0){
		perror("Failed to send data.");
		exit(EXIT_FAILURE);
	}
		
	// ACK packet - without considering several blocks
	//ssize_t nbackpck = 4; 
	/*
	buf_ackdata[0] = 0;
	buf_ackdata[1] = 4;
	buf_ackdata[2] = 0;
	buf_ackdata[3] = 1;	
	*/
		
	// Reception 
	char buf_receivedData[LEN_DATA + 4] = {}; 
	size_t len_buf_receivedData = LEN_DATA + 4; //512 bytes
	
	struct sockaddr_in ServerResponse; // to stock the ip address and port number for sending ACK
	socklen_t ServerResponseLen = sizeof(ServerResponse); 

	/*
	 2 bytes     2 bytes      n bytes
	------------------------------------
	| Opcode |   Block #  |   Data     |
	------------------------------------
	     16 bytes	    20 bytes	(n+4) bytes  4 bytes
	-----------------------------------------------------
	|  Local Medium  |  Internet  |  Datagram  |  TFTP  |
	-----------------------------------------------------
	*/

	int blockNumber = 0;
	ssize_t nbackpck = sizeof(buf_ackdata);
	
	do {
		rcvFrom = recvfrom(clientSocket, buf_receivedData, len_buf_receivedData, 0, (struct sockaddr *)&ServerResponse, &ServerResponseLen);
		if (rcvFrom < 0){
			perror("Receive data failed.");
			exit(EXIT_FAILURE);
		}		
		
		blockNumber = (buf_receivedData[2] << 8) | buf_receivedData[3];
		buf_ackdata[0] = 0x00;
		buf_ackdata[1] = 0x04; 							// Opcode for ACK
		buf_ackdata[2] = (blockNumber >> 8) & 0xFF; // High byte of BlockNumber
	    buf_ackdata[3] = blockNumber & 0xFF;        // Low byte of BlockNumber		
		
		sndto = sendto(clientSocket, buf_ackdata, nbackpck, MSG_EOR, (struct sockaddr *)&ServerResponse, ServerResponseLen);
		if (sndto < 0){
			perror("ACK failed.");
			exit(EXIT_FAILURE);
		}
		if (rcvFrom == LEN_DATA){
			rcvFrom = recvfrom(clientSocket, buf_receivedData, len_buf_receivedData, 0, (struct sockaddr *)&ServerResponse, &ServerResponseLen);
			if (rcvFrom < 0){
				perror("Receive data failed.");
				exit(EXIT_FAILURE);
			}
		}
		printf("In While : rcvFrom = %ld\n", rcvFrom);
	} while(rcvFrom == LEN_DATA + 4);
	
	/*
	while(rcvFrom == LEN_DATA){
		sndto = sendto(clientSocket, buf_ackdata, nbackpck, MSG_EOR, (struct sockaddr *)&ServerResponse, ServerResponseLen);
		if (sndto < 0){
			perror("ACK failed.");
			exit(EXIT_FAILURE);
		}
		rcvFrom = recvfrom(clientSocket, buf_receivedData, len_buf_receivedData, 0, (struct sockaddr *)&ServerResponse, &ServerResponseLen);
		if (rcvFrom < 0){
				perror("Receive data failed.");
				exit(EXIT_FAILURE);
		}
		printf("In While : rcvFrom = %ld\n", rcvFrom);
	}

	sndto = sendto(clientSocket, buf_ackdata, nbackpck, MSG_EOR, (struct sockaddr *)&ServerResponse, ServerResponseLen);
	if (sndto < 0){
		perror("ACK failed o.");
		exit(EXIT_FAILURE);
	}
	*/
	
	
	printf("Client ended.\n");
	
}
