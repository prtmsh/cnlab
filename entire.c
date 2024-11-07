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



// DHCP:
// The router R3 is configured as the DHCP server by the following commands:
// R3> conf t
// R3> ip dhcp pool nw1
// R3> network 192.168.10.0 255.255.255.0
// R3> default-router 192.168.10.254
// R3> exit

// R3> conf t
// R3> ip dhcp pool nw2
// R3> network 172.16.0.0 255.255.0.0
// R3> default-router 172.16.255.254
// R3> exit

// R3> conf t 
// R3> ip dhcp pool nw3
// R3> network 192.168.20.0 255.255.255.0
// R3> default-router 192.168.20.254
// R3> exit

// Now, running the command DHCP at each PC will lead to the DHCP assigning the IP address automatically. 


// VLAN:
// For VLAN1 (with PC1 and PC2)
// At R1,
// # int f0/0.1
// # encapsulation dot1q 1
// # ip address DEFAULT_GATWAY MASK

// For VLAN2 (with PC3 and PC4)
// At R2,
// # int f0/0.2
// # encapsulation dot1q 2
// # ip address DEFAULT_GATEWAY MASK




// DNS:
// To configure R3 as a DNS server,
// R3> conf t
// R3> ip dns server
// R3> ip name-server 20.20.20.2
// R3> ip domain-name prathamesh.com
// R3> ip dhcp pool nw1
// R3> dns-server 20.20.20.2
// R3> ip host PC1.prathamesh.com 192.168.10.1
// R3> ip host PC2.prathamesh.com 192.168.10.2
// R3> ip host PC3.prathamesh.com 172.16.0.3
// R3> ip host PC4.prathamesh.com 172.16.0.4
// R3> ip host PC5.prathamesh.com 192.168.20.1

// Then, at every other router (R1, R2 in this case):
// R1> ip name-server 20.20.20.2
// R2> ip name-server 20.20.20.2
