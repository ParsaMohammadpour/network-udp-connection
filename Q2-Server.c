#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

int main(void){
	int maxClientNumber = 10; // Maximum Number Of Connection At Time
	int connectionCounter = 1;
	
    // Making Socket
    int socketDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(socketDescriptor < 0){
        printf("Error while creating socket!\n");
        return -1;
    }
    printf("Pure Socket Is Created.\n");
    
    // Set port and IP:
	struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(2000);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // Bind to the set port and IP:
	int serverAddressSize = sizeof(serverAddress);
    if(bind(socketDescriptor, (struct sockaddr*)&serverAddress, serverAddressSize) < 0){
        printf("Couldn't bind to the port\n");
        return -1;
    }
    printf("Socket Details Was Added Successfuly\n");
	printf("Start Listening For Request...\n");
	
	while(1){
		// Making Buffers To Send & Receive
		char serverBuffer[1000], clientBuffer[1000];
		int bufferSize = sizeof(serverBuffer);
		memset(serverBuffer, '\0', bufferSize);
		memset(clientBuffer, '\0', bufferSize);
		
		// Receive Client Message From Client
		struct sockaddr_in clientAddress;
		int clientSize = sizeof(clientAddress);
		if (recvfrom(socketDescriptor, clientBuffer, bufferSize, 0,
			(struct sockaddr*)&clientAddress, &clientSize) < 0){
			printf("Error On Receiving Client Message!\n");
			return -1;
		}
		printf("---------------Connection Number %d---------------\n", connectionCounter++);
		printf("Client IP & Port: %s:%i\n",
			inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
		printf("DNS: %s\n", clientBuffer);
		
		// Getting Address
		struct hostent *host = gethostbyname(clientBuffer);
		struct in_addr a;
		while (*host->h_addr_list != NULL)
		{
			bcopy(*host->h_addr_list++, (char *) &a, sizeof(a));
			char*address = inet_ntoa(a);
			printf("address: %s\n", address);
			strcat(serverBuffer, address);
			strcat(serverBuffer, "\n");
		}
		
		// Send Response To Client
		if (sendto(socketDescriptor, serverBuffer, strlen(serverBuffer), 0,
			(struct sockaddr*)&clientAddress, clientSize) < 0){
			printf("Error On Sending Message To Client!\n");
			return -1;
		}
	}
    
    // Closing Server Socket
    close(socketDescriptor);
    return 0;
}