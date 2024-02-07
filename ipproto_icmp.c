#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
 
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
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    printf("Raw socket created successfully.\n");

    // Packet to be sent (assuming raw data)
    const char *packet_data = "PingData";  // Replace with your ping packet data

    // IP header structure
    struct iphdr ip_header;
    ip_header.version = 4;      // IPv4
    ip_header.ihl = 5;          // Header length (in 32-bit words)
    ip_header.tos = 0;          // Type of service
    ip_header.tot_len = htons(sizeof(struct iphdr) + sizeof(struct icmphdr) + strlen(packet_data));  // Total length
    ip_header.id = htons(12345);  // Identification
    ip_header.frag_off = 0;       // Fragmentation offset
    ip_header.ttl = 64;           // Time-to-live
    ip_header.protocol = IPPROTO_ICMP;  // Protocol (ICMP)
    ip_header.check = 0;              // Checksum (filled later)
    ip_header.saddr = inet_addr("192.168.0.1");  // Source IP address
    ip_header.daddr = inet_addr("8.8.8.8");  // Destination IP address

    // ICMP header structure
    struct icmphdr icmp_header;
    icmp_header.type = ICMP_ECHO;     // ICMP Echo Request
    icmp_header.code = 0;             // Code for Echo Request
    icmp_header.checksum = 0;         // Checksum (filled later)
    icmp_header.un.echo.id = htons(1); // Identifier
    icmp_header.un.echo.sequence = htons(1);  // Sequence number

    // Calculate and fill in the checksum for IP and ICMP headers
    ip_header.check = csum((unsigned short*)&ip_header, sizeof(struct iphdr));
    icmp_header.checksum = csum((unsigned short*)&icmp_header, sizeof(struct icmphdr) + strlen(packet_data));

    // Assemble the packet
    char packet[sizeof(struct iphdr) + sizeof(struct icmphdr) + strlen(packet_data)];
    memcpy(packet, &ip_header, sizeof(struct iphdr));
    memcpy(packet + sizeof(struct iphdr), &icmp_header, sizeof(struct icmphdr));
    memcpy(packet + sizeof(struct iphdr) + sizeof(struct icmphdr), packet_data, strlen(packet_data));

    for (size_t i = 0; i < sizeof(packet); ++i) {
        for (int bit = 7; bit >= 0; --bit) {
          
printf("packet[i] & (1<< bit) \n");        
printf("packet[i]: ");
for (int j = 7; j >= 0; --j) {
    printf("%d", (packet[i] & (1 << j)) ? 1 : 0);
}
printf("\n");
char x = (1<<bit);
printf("1 <<bit  : ");
for (int j = 7; j >= 0; --j) {
    printf("%d", (x & (1 << j)) ? 1 : 0);
}
printf("\n");


printf("\n");           
                   
     
            //putchar((packet[i] & (1 << bit)) ? '1' : '0');
        }
        putchar('\n'); // Optional: Add a space for better readability
    }
    putchar('\n');


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
