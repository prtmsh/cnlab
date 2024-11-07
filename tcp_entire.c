//client.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define portno 10205

struct clientData{
	char name[50];
	int regno;
};

struct serverData{
	char response[50];
	int status;
};

int main(){
	int sockFD;
	struct sockaddr_in seraddr;
	struct clientData cData;
	struct serverData sData;
	
	sockFD = socket(AF_INET, SOCK_STREAM, 0);
	
	seraddr.sin_family = AF_INET;
	seraddr.sin_addr.s_addr = inet_addr("10.145.1.119");
	seraddr.sin_port = htons(portno);
	
	connect(sockFD, (struct sockaddr*)&seraddr, sizeof(seraddr));
	
	printf("enter your name: ");
	scanf("%s", cData.name);
	
	printf("enter your regno: ");
	scanf("%d", &cData.regno);
	
	write(sockFD, &cData, sizeof(cData));
	
	read(sockFD, &sData, sizeof(sData));
	
	printf("server response. response: %s, status: %d", sData.response, sData.status);
	
	close(sockFD);
	
	return 0;
}


//server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#define portno 10205

struct clientData{
	char name[50];
	int regno;
};

struct serverData{
	char response[50];
	int status;
};

int main(){
	int sockFD, newSockFD, cliLen;
	struct sockaddr_in seraddr, cliaddr;
	struct clientData cData;
	struct serverData sData;
	
	sockFD = socket(AF_INET, SOCK_STREAM, 0);
	
	seraddr.sin_family = AF_INET;
	seraddr.sin_addr.s_addr = inet_addr("10.145.1.119");
	seraddr.sin_port = htons(portno);
	
	bind(sockFD, (struct sockaddr*)&seraddr, sizeof(seraddr));
	
	listen(sockFD, 10);
	
	while(1){
		cliLen = sizeof(cliaddr);
		newSockFD = accept(sockFD, (struct sockaddr*)&cliaddr, &cliLen);
		if(fork()==0){
			read(newSockFD, &cData, sizeof(cData));
			printf("received data from client. name: %s, regno: %d", cData.name, cData.regno);
			
			strcpy(sData.response, cData.name);
			sData.status = 1;
			write(newSockFD, &sData, sizeof(sData));
			
			close(newSockFD);
			exit(0);
		}
		else{
			close(newSockFD);
		}
	}
	
	close(sockFD);
}
