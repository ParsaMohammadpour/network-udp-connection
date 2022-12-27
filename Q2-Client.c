#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void){
	// Get input from the user
	char dns[100];
    printf("Enter message: ");
    gets(dns);
    
    // Creating Pure Socket
    int socketDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(socketDescriptor < 0){
        printf("Error On Creating Socket!\n");
        return -1;
    }
    printf("Socket Was created.\n");
    
    // Setting Server Address & Port
	struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(2000);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	// Making Buffer To Send & Receive
	char serverBuffer[1000], clientBuffer[1000];
    memset(serverBuffer, '\0', sizeof(serverBuffer));
    memset(clientBuffer, '\0', sizeof(clientBuffer));
	strcpy(clientBuffer, dns);
    
    // Sending Message To Server
	int serverAddressSize = sizeof(serverAddress);
    if(sendto(socketDescriptor, clientBuffer, strlen(clientBuffer), 0,
         (struct sockaddr*)&serverAddress, serverAddressSize) < 0){
        printf("Error On Sending Message To Server!\n");
        return -1;
    }
    
    // Receive Server Message
	int serverBufferSize = sizeof(serverBuffer);
    if(recvfrom(socketDescriptor, serverBuffer, serverBufferSize, 0,
         (struct sockaddr*)&serverAddress, &serverAddressSize) < 0){
        printf("Error On Receiving Message From Server!\n");
        return -1;
    }
    printf("Server's response:\n%s", serverBuffer);
	
    // Closing Socket
    close(socketDescriptor);
    return 0;
}