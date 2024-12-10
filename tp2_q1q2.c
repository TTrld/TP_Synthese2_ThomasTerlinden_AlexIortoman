#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 250

int main(int argc, char *argv[]){
	
	int s;
	int e;
	struct addrinfo hints;
	struct addrinfo *res, *r;
	struct sockaddr *addr;
	
	char *serveraddr = argv[1];
	char *filename = argv[2];
	char *port = argv[3];
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_protocol = IPPROTO_UDP;
	hints.ai_socktype = SOCK_DGRAM;
	
	s = getaddrinfo(serveraddr, port, &hints, &res);
	addr = hints.ai_addr;
	
	sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	
	if (e != 0){
		fprintf(stderr, "Erreur de GETADDRINFO%s\n", gai_strerror(e));
		exit(EXIT_FAILURE);
	}
	
	for (r = res; r!= NULL; r = r->ai_next){
		printf("famille = %d, Type Socket = %d, protocol = %d\n", r->ai_family, r->ai_socktype, r->ai_protocol);
	}
	//printf("IP: %s\n", addr);
	
	freeaddrinfo(res);
	exit(EXIT_SUCCESS);

}



