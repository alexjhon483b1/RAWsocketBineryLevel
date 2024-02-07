This project focuses on studying the socket and tcp ip in binery level.


1. **TCP (SOCK_STREAM):**
   ```c
   int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
   ```

2. **UDP (SOCK_DGRAM):**
   ```c
   int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
   ```

3. **Raw IP Packet (SOCK_RAW, IPPROTO_RAW):**
   ```c
   int raw_ip_socket = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
   ```

4. **ICMP (SOCK_RAW, IPPROTO_ICMP):**
   ```c
   int icmp_socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
   ```

These socket types cover different network protocols and communication styles:

- `SOCK_STREAM` is for reliable, connection-oriented communication using TCP.
- `SOCK_DGRAM` is for connectionless communication using UDP.
- `SOCK_RAW` with `IPPROTO_RAW` is for manually crafting raw IP packets.
- `SOCK_RAW` with `IPPROTO_ICMP` is commonly used for crafting and sending ICMP messages, often used in tools like ping.

These socket types provide flexibility for different network programming needs.

In addition to TCP, UDP, raw IP packets, and ICMP, there are other protocols and packet types that you might encounter in networking. Here are a few examples:

1. **ICMPv6 (IPv6):**
   ```c
   int icmpv6_socket = socket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6);
   ```
   Similar to ICMP for IPv4, this socket type is used for crafting and sending ICMPv6 messages in IPv6 networks.

2. **TCP and UDP with IPv6:**
   ```c
   int tcpv6_socket = socket(AF_INET6, SOCK_STREAM, 0);
   int udpv6_socket = socket(AF_INET6, SOCK_DGRAM, 0);
   ```
   Similar to IPv4, these socket types are for TCP and UDP communication, but in IPv6 networks.

3. **Raw Ethernet Frames:**
   ```c
   int raw_eth_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
   ```
   This socket type is used for working with raw Ethernet frames. It allows you to send and receive Ethernet frames directly, including crafting custom link-layer protocols.

4. **Multicast:**
   ```c
   int multicast_socket = socket(AF_INET, SOCK_DGRAM, 0);
   ```
   For joining and participating in multicast communication.

5. **SCTP (Stream Control Transmission Protocol):**
   ```c
   int sctp_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
   ```
   Similar to TCP, SCTP is a reliable, connection-oriented transport protocol.

6. **Raw Packet Capture (using pcap):**
   ```c
   pcap_t *handle = pcap_open_live("eth0", BUFSIZ, 1, 1000, errbuf);
   ```
   For capturing raw packets from a network interface.








The code provided is a C function for setting the IP address of a network interface on a Linux/Unix system. Let's break down the key components:

```c
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
```

This section includes the necessary header files for socket programming, ioctl operations, network interfaces, and Internet protocols.

```c
int setIpAddress(const char *interface, const char *ipAddress) {
    int sockfd;
    struct ifreq ifr;
    struct sockaddr_in addr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }
```

Here, the function `setIpAddress` takes two parameters: `interface` (network interface name, e.g., "eth0") and `ipAddress` (the IP address to be assigned).

The function begins by creating a socket using the `socket` system call with the address family `AF_INET` (IPv4) and socket type `SOCK_DGRAM` (datagram socket).

```c
    strncpy(ifr.ifr_name, interface, IFNAMSIZ);

    // Set the IP address
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ipAddress);
    memcpy(&ifr.ifr_addr, &addr, sizeof(struct sockaddr));
```

This part of the code populates the `ifr` structure with the network interface name (`ifr_name`) and sets the IP address. It uses the `inet_addr` function to convert the IP address string to the binary form expected by `sin_addr.s_addr`.

```c
    if (ioctl(sockfd, SIOCSIFADDR, &ifr) == -1) {
        perror("ioctl SIOCSIFADDR");
        close(sockfd);
        return -1;
    }
```

The `ioctl` system call is then used with the `SIOCSIFADDR` command to set the IP address of the specified network interface.

```c
    close(sockfd);
    return 0;
}
```

Finally, the socket is closed, and the function returns `0` if the operation was successful. If there is an error, it prints an error message using `perror` and returns `-1`.





tools that had been used: 

wireshirk 

tcpdump 

scapy

sites that had been used:

hex to binery

hex packet analysis

concepts learnt from youtube:

checksum 

bitwise operator

bitwise left operator


linux command used:

sudo modprobe dummy         # Load the dummy kernel module

sudo ip link add dummy0 type dummy  # Create a dummy interface named dummy0

sudo ip link set up dummy0   # Bring the dummy interface up

sudo ip link set down dummy0   # Bring the dummy interface down

sudo ip link delete dummy0     # Delete the dummy interface

iptable rules:

sudo iptables -L

sudo iptables -A OUTPUT -p tcp --dport 80 -j ACCEPT

sudo iptables -A INPUT -p tcp --dport 25 -j DROP

sudo iptables -D INPUT -p tcp --dport 25 -j DROP

sudo iptables -D OUTPUT -p tcp --dport 80 -j ACCEPT

