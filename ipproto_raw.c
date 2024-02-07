#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>

// Function to calculate the checksum
unsigned short csum(unsigned short *ptr, int nbytes) {
    register long sum;
    unsigned short oddbyte;
    register short answer;

    sum = 0;
    while (nbytes > 1) {
        sum += *ptr++;
        nbytes -= 2;
    }

    if (nbytes == 1) {
        oddbyte = 0;
        *((u_char*)&oddbyte) = *(u_char*)ptr;
        sum += oddbyte;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = (short)~sum;

    return answer;
}

int main() {
    // Create raw socket
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    printf("Raw socket created successfully.\n");

    // Packet to be sent (assuming raw data)
    const char *packet_data = "RawPacketData";  // Replace with your raw packet data

    // IP header structure
    struct iphdr ip_header;
    ip_header.version = 4;  // IPv4
    ip_header.ihl = 5;      // Header length (in 32-bit words)
    ip_header.tos = 0;      // Type of service
    ip_header.tot_len = sizeof(struct iphdr) + strlen(packet_data);
    ip_header.id = htons(12345);  // Identification
    ip_header.frag_off = 0;       // Fragmentation offset
    ip_header.ttl = 64;           // Time-to-live
    ip_header.protocol = IPPROTO_RAW;  // Protocol (raw)
    ip_header.check = 0;              // Checksum (filled later)
    ip_header.saddr = inet_addr("192.168.0.1");  // Source IP address
    ip_header.daddr = inet_addr("103.221.255.104");  // Destination IP address

    // Calculate and fill in the checksum
    ip_header.check = csum((unsigned short*)&ip_header, sizeof(struct iphdr));

    // Assemble the packet
    char packet[sizeof(struct iphdr) + strlen(packet_data)];
    memcpy(packet, &ip_header, sizeof(struct iphdr));
    memcpy(packet + sizeof(struct iphdr), packet_data, strlen(packet_data));

    // Send the packet
    if (sendto(sock, packet, sizeof(packet), 0, (struct sockaddr*)&(struct sockaddr_in){.sin_addr.s_addr = ip_header.daddr, .sin_family = AF_INET}, sizeof(struct sockaddr_in)) == -1) {
        perror("sendto");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Packet sent successfully.\n");

    // Close the socket
    close(sock);

    return 0;
}

