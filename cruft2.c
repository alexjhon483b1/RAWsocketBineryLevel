#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 
int main() {
    // Create raw socket
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    printf("Raw socket created successfully.\n");

    // Set destination IP address and port
    const char *dest_ip = "103.221.255.104";  // Replace with the destination IP address
    const int dest_port = 80;           // Replace with the destination port

    // Create destination sockaddr_in structure
    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.s_addr = inet_addr(dest_ip);
    dest_addr.sin_port = htons(dest_port);

    // Message to be sent
    const char *message = "hello";

    // Send the message using sendto
    if (sendto(sock, message, strlen(message), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr)) == -1) {
        perror("sendto");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Message sent successfully.\n");

    // Close the socket
    close(sock);

    return 0;
}

