#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 300

int main(int argc, char *argv[]){
	
	int s, c, w, e;
	int sfd;
	int sockfd;
	char *buff_senddata[BUF_SIZE];
	char *buff_server[BUF_SIZE]; 
	struct addrinfo hints;
	struct addrinfo *res, *r;
	struct sockaddr *addr;
	struct sockadd_in;
	
	char *serveraddr = argv[1];
	char *filename = argv[2];
	char *port = argv[3];
	
	//init + config of hints
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_protocol = IPPROTO_UDP;
	hints.ai_socktype = SOCK_DGRAM;
	
	s = getaddrinfo(serveraddr, port, &hints, &res);
	//int nameinfo = getnameinfo(res->ai_addr, res.ai_addrlen, buff_senddata, sizeof(buff_senddata), buff_server, sizeof(buff_server), NI_NUMERICHOST | NI_NUMERICSERV);
	/*
	if (nameinfo == -1){
		perror("getnameinfo error");
		exit(EXIT_FAILURE);
	}
	*/
	//getaddrinfo errors
	if (s == -1){
		fprintf(stderr, "Erreur de GETADDRINFO%s\n", gai_strerror(e));
		exit(EXIT_FAILURE);
	}
	
	//Socket config 
	sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sfd < 0){
 		perror("socket error");
		exit(EXIT_FAILURE);
	}
	
	char *octet = "octet";
	buff_senddata[0] = "0";
	buff_senddata[1] = "1";
	for [int i = 2; i<strlen(filename); i++){
		buff_senddata[i] = filename[i];
	buff_senddata[0+1+strlen(filename)] = "0";
	for (int j = 0; j<strlen(octet); j++){
		buff_senddata[0+1+strlen(filename) + j+1] = octet[j];
	buff_senddata[0+1+strlen(filename) + strlen(octet)+1] = "0";
	 
	c = sendto(sfd, buff_senddata, strlen(buff_senddata), 0, (struct sockaddr*) res->ai_addr, sizeof(res->ai_addr));
	if (c == -1){
		perror("sendto error");
		exit(EXIT_FAILURE);
	}
	/*
	w = write(sfd, buff_senddata, BUF_SIZE);
	if (w == -1){
		perror("write error");
		exit(EXIT_FAILURE);
	}
	*/
		
	//Displaying
	for (r = res; r!= NULL; r = r->ai_next){
		printf("famille = %d, Type Socket = %d, protocol = %d\n", r->ai_family, r->ai_socktype, r->ai_protocol);
	}
	//printf("IP: %s\n", addr);
	
	freeaddrinfo(res);
	exit(EXIT_SUCCESS);

}



