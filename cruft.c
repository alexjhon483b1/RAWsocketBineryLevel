#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>

int main() {
    // Create raw socket
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    printf("Raw socket created successfully.\n");

    // Close the socket
    close(sock);

    return 0;
}

