#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

int main() {
    int socketDescriptor;
    struct sockaddr_in serverAddress;
    char sendBuffer[8000], recvBuffer[8000];
    pid_t cpid;

    bzero(&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(9652);

    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);

    if (connect(socketDescriptor, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Error connecting to the server");
        exit(EXIT_FAILURE);
    }
    cpid = fork();

    if (cpid == 0) {
        while (1) {
            bzero(&sendBuffer, sizeof(sendBuffer));
            printf("\nType a message here ... ");
            fgets(sendBuffer, sizeof(sendBuffer), stdin);
            send(socketDescriptor, sendBuffer, strlen(sendBuffer), 0);
            printf("\nMessage sent!\n");
        }
    } else {
        while (1) {
            bzero(&recvBuffer, sizeof(recvBuffer));
            recv(socketDescriptor, recvBuffer, sizeof(recvBuffer), 0);
            printf("\nSERVER : %s\n", recvBuffer);
        }
    }

    close(socketDescriptor);

    return 0;
}

