#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORT 5214
#define MAX 1000

int main() {
    char clientResponse[MAX];
    char serverResponse[MAX];

    int socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);

    if (socketDescriptor < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    char hostname[MAX];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        struct hostent *hostIP = gethostbyname(hostname);

        if (hostIP == NULL) {
            printf("ERROR: IP Address Not Found\n");
            exit(EXIT_FAILURE);
        }

        struct sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(PORT);
        serverAddress.sin_addr = *(struct in_addr *)hostIP->h_addr_list[0];

        if (connect(socketDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
            perror("Error connecting to the server");
            exit(EXIT_FAILURE);
        }

        printf("\nLocalhost: %s\n", inet_ntoa(*(struct in_addr *)hostIP->h_addr_list[0]));
        printf("Local Port: %d\n", PORT);
        printf("Remote Host: %s\n", inet_ntoa(serverAddress.sin_addr));

        while (1) {
            recv(socketDescriptor, serverResponse, sizeof(serverResponse), 0);
            printf("\nSERVER : %s", serverResponse);

            printf("\nText message here: ");
            scanf("%s", clientResponse);
            send(socketDescriptor, clientResponse, strlen(clientResponse), 0);
        }
    } else {
        perror("Error getting hostname");
        exit(EXIT_FAILURE);
    }

    close(socketDescriptor);
    return 0;
}
 
