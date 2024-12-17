#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 32

int main(int argc, char *argv[]){
	
	printf("Client started.\n");
	
	const char *hostName = argv[1];
	const char *fileName = argv[2];
	const char *portNumber = argv[3];  //1069
	
	int clientSocket; 
	ssize_t sndto; 
	struct addrinfo hints; 
	struct addrinfo *results; 
	struct addrinfo *record;
	
	char buf_senddata[BUF_SIZE] = {};
		
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
	buf_senddata[0] = '0'; // Opcode 1/2
	buf_senddata[1] = '1'; // Opcode 2/2 : Read request (RRQ)
	for (int i = 0; i<strlen(fileName); i++){ // Filename
		buf_senddata[i+2] = fileName[i];
	}
	buf_senddata[2 + strlen(fileName)] = '0';
	for (int i = 0; i<strlen(mode); i++){ // Mode
		buf_senddata[2 + strlen(fileName) + 1 + i] = mode[i];
	}
	buf_senddata[3 + strlen(fileName) + strlen(mode)] = '0';
		
	//char *sendDataTab = (char *)malloc((3 + strlen(fileName) + strlen(mode)) * sizeof(char));
	/*
	for (int i = 0; i<4 + strlen(fileName) + strlen(mode); i++){
		senddata[i] = buf_senddata[i];
		//printf("senddata[%d] = %c\n", i, senddata[i]);
	}
	*/
	
	sndto = sendto(clientSocket, buf_senddata, strlen(buf_senddata), MSG_EOR, results->ai_addr, results->ai_addrlen);
	if (sndto < 0){
		perror("Failed to send data.");
		exit(EXIT_FAILURE);
	}

	//free(sendDataTab);
	printf("Client ended.\n");
	
}
