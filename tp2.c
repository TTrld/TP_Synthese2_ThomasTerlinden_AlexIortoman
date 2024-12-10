#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>



int main(int argc, char *argv[]){
	
	int s;
	int e;
	struct addrinfo hints;
	struct addrinfo *res, *r;
	struct sockaddr *addr;
	
	char *serveraddr = argv[1];
	char *filename = argv[2];
	char *port = argv[3];
	
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_protocol = IPPROTO_UDP;
	
	s = getaddrinfo(serveraddr, NULL, &hints, &res);
	//addr = getnameinfo(
	//addr = hints->ai_addr;
	
	if (e != 0){
		fprintf(stderr, "Erreur de GETADDRINFO%s\n", gai_strerror(e));
		exit(EXIT_FAILURE);
	}
	
	for (r = res; r!= NULL; r = r->ai_next){
		printf("famille = %d, Type Socket = %d, protocol = %d\n", r->ai_family, r->ai_socktype, r->ai_protocol);
	}
	
	freeaddrinfo(res);
	exit(EXIT_SUCCESS);

}



