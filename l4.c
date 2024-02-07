#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

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
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set IP headers
    struct iphdr iph;
    iph.ihl = 5;
    iph.version = 4;
    iph.tos = 0;
    iph.tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr);
    iph.id = htons(54321);
    iph.frag_off = 0;
    iph.ttl = 255;
    iph.protocol = IPPROTO_TCP;
    iph.check = 0; // Will be filled later
    iph.saddr = inet_addr("192.168.0.104"); // Source IP address
    iph.daddr = inet_addr("103.221.255.104"); // Destination IP address

    // Set TCP headers
    struct tcphdr tcph;
    tcph.source = htons(1234); // Source port
    tcph.dest = htons(80); // Destination port
    tcph.seq = 0;
    tcph.ack_seq = 0;
    tcph.res1 = 0;
    tcph.doff = 5;
    tcph.fin = 0;
    tcph.syn = 1; // SYN packet
    tcph.rst = 0;
    tcph.psh = 0;
    tcph.ack = 0;
    tcph.urg = 0;
    tcph.window = htons(5840);
    tcph.check = 0; // Will be filled later
    tcph.urg_ptr = 0;

    // Fill IP header checksum
    iph.check = csum((unsigned short*)&iph, sizeof(struct iphdr));

    // Pseudo-header for TCP checksum calculation
    struct pseudo_header {
        unsigned int source_address;
        unsigned int dest_address;
        unsigned char placeholder;
        unsigned char protocol;
        unsigned short tcp_length;
        struct tcphdr tcp;
    } psh;

    psh.source_address = iph.saddr;
    psh.dest_address = iph.daddr;
    psh.placeholder = 0;
    psh.protocol = IPPROTO_TCP;
    psh.tcp_length = htons(sizeof(struct tcphdr));

    // Copy TCP header to pseudo-header
    memcpy(&psh.tcp, &tcph, sizeof(struct tcphdr));

    // Fill TCP header checksum
    tcph.check = csum((unsigned short*)&psh, sizeof(struct pseudo_header));

    // Set socket options to include IP headers
    int one = 1;
    const int *val = &one;
    if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0) {
        perror("setsockopt");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Send the packet
    if (sendto(sock, &iph, sizeof(struct iphdr), 0, (struct sockaddr*)&(struct sockaddr_in){.sin_addr.s_addr = iph.daddr, .sin_family = AF_INET}, sizeof(struct sockaddr_in)) < 0) {
        perror("sendto");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("SYN packet sent successfully.\n");

    // Close the socket
    close(sock);

    return 0;
}
