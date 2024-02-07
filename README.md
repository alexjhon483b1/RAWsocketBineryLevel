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

