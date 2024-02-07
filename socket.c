#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/in.h>

#define DEST_IP "103.221.255.104"  // Update with the IP address of your server
#define PORT 443
#define PACKET_SIZE 1024

// Function to calculate the checksum of a packet
unsigned short calculate_checksum(unsigned short *buffer, int size) {
    unsigned long cksum = 0;

    while (size > 1) {
        cksum += *buffer++;
        size -= sizeof(unsigned short);
    }

    if (size) {
        cksum += *(unsigned char *)buffer;
    }

    cksum = (cksum >> 16) + (cksum & 0xFFFF);
    cksum += (cksum >> 16);

    return (unsigned short)(~cksum);
}

int main() {
    int raw_socket;
    struct sockaddr_in dest_addr;
    char buffer[PACKET_SIZE] = "Hello, server! This is the raw socket client.";

    // Create a raw socket
    if ((raw_socket = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Fill in the destination address structure
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, DEST_IP, &dest_addr.sin_addr) <= 0) {
        perror("Invalid destination IP");
        exit(EXIT_FAILURE);
    }

    // Prepare the IP header
    struct iphdr *ip_header = (struct iphdr *)buffer;

    ip_header->ihl = 5;
    ip_header->version = 4;
    ip_header->tos = 0;
    ip_header->tot_len = sizeof(struct iphdr) + strlen(buffer);
    ip_header->id = htons(443);
    ip_header->frag_off = 0;
    ip_header->ttl = 64;
    ip_header->protocol = IPPROTO_RAW; // Custom protocol (raw)
    ip_header->check = 0; // Checksum calculated later
    ip_header->saddr = inet_addr("192.168.0.104"); // Update with the source IP if necessary
    ip_header->daddr = dest_addr.sin_addr.s_addr;

    // Calculate and set the IP header checksum
    ip_header->check = calculate_checksum((unsigned short *)ip_header, ip_header->tot_len);

    // Send the packet
    if (sendto(raw_socket, buffer, ip_header->tot_len, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) == -1) {
        perror("Packet sending failed");
        exit(EXIT_FAILURE);
    }

    printf("Packet sent to the server.\n");

    close(raw_socket);

    return 0;
}

