#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORT 6969
#define BACKLOG 5

int main() {
    int size;
    int socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientLength;
    struct stat statVariable;
    char buffer[100], file[1000];
    FILE *filePointer;

    bzero(&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(PORT);

    bind(socketDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    listen(socketDescriptor, BACKLOG);

    printf("%s\n", "Server is running ...");

    clientLength = sizeof(clientAddress);
    int clientDescriptor = accept(socketDescriptor, (struct sockaddr*)&clientAddress, &clientLength);

    while (1) {
        bzero(buffer, sizeof(buffer));
        bzero(file, sizeof(file));

        recv(clientDescriptor, buffer, sizeof(buffer), 0);

        filePointer = fopen(buffer, "r");
        if (filePointer == NULL) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        stat(buffer, &statVariable);
        size = statVariable.st_size;

        fread(file, size, 1, filePointer);
        fclose(filePointer);

        send(clientDescriptor, file, size, 0);
    }

    close(socketDescriptor);
    close(clientDescriptor);

    return 0;
}
