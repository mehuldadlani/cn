#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>

#define MAX_BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int sd;
    char buff[MAX_BUFFER_SIZE];
    struct sockaddr_in servaddr;
    socklen_t len = sizeof(servaddr);

    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0) {
        perror("Cannot open socket");
        exit(1);
    }

    bzero(&servaddr, len);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(5669);

    while (1) {
        printf("Enter Input data: \n");
        bzero(buff, sizeof(buff));

        fgets(buff, sizeof(buff), stdin);

        if (sendto(sd, buff, sizeof(buff), 0, (struct sockaddr*)&servaddr, len) < 0) {
            perror("Cannot send data");
            exit(1);
        }

        printf("Data sent to UDP Server: %s", buff);
        bzero(buff, sizeof(buff));

        /* Receiving the echoed message from the server */
        if (recvfrom(sd, buff, sizeof(buff), 0, (struct sockaddr*)&servaddr, &len) < 0) {
            perror("Cannot receive data");
            exit(1);
        }

        printf("Received Data from server: %s", buff);
    }

    close(sd);
    return 0;
}
