#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <unistd.h>
#include <net/if.h>
#include <cstring>
#include <netinet/ip.h>
#include <iomanip>
#include <netinet/tcp.h>
#include <netinet/udp.h>

// Helper function to print known protocol details
void printProtocolDetails(int sourcePort, int destPort, const char* payload, int payloadLength) {
    // Identify protocol based on port numbers and payload signatures
    if (sourcePort == 80 || destPort == 80 || (payloadLength > 0 && strncmp(payload, "HTTP", 4) == 0)) {
        std::cout << "Protocol: HTTP" << std::endl;
    } else if (sourcePort == 443 || destPort == 443) {
        std::cout << "Protocol: HTTPS" << std::endl;
    } else if (sourcePort == 21 || destPort == 21) {
        std::cout << "Protocol: FTP Control" << std::endl;
    } else if (sourcePort == 20 || destPort == 20) {
        std::cout << "Protocol: FTP Data" << std::endl;
    } else if (sourcePort == 25 || destPort == 25) {
        std::cout << "Protocol: SMTP" << std::endl;
    } else if (sourcePort == 53 || destPort == 53) {
        std::cout << "Protocol: DNS" << std::endl;
    } else if (sourcePort == 22 || destPort == 22) {
        std::cout << "Protocol : SSH" << std::endl;
    } else {
        std::cout << "Protocol: Unknown" << std::endl;
    }
}

void printPacketInfo(const char* buffer, int length) {
    struct iphdr* ipHeader = (struct iphdr*)buffer;
    int ipHeaderLength = ipHeader->ihl * 4;

    // Check if the packet contains an IPv4 header
    if (ipHeader->version == 4) {
        std::cout << "Source IP: " << inet_ntoa(*(struct in_addr*)&ipHeader->saddr) << std::endl;
        std::cout << "Destination IP: " << inet_ntoa(*(struct in_addr*)&ipHeader->daddr) << std::endl;

        if (ipHeader->protocol == IPPROTO_TCP) {
            struct tcphdr* tcpHeader = (struct tcphdr*)(buffer + ipHeaderLength);
            int tcpHeaderLength = tcpHeader->doff * 4;

            const char* payload = buffer + ipHeaderLength + tcpHeaderLength;
            int payloadLength = length - ipHeaderLength - tcpHeaderLength;

            int sourcePort = ntohs(tcpHeader->source);
            int destPort = ntohs(tcpHeader->dest);

            std::cout << "Source Port: " << sourcePort << std::endl;
            std::cout << "Destination Port: " << destPort << std::endl;

            printProtocolDetails(sourcePort, destPort, payload, payloadLength);
        } else if (ipHeader->protocol == IPPROTO_UDP) {
            struct udphdr* udpHeader = (struct udphdr*)(buffer + ipHeaderLength);
            const char* payload = buffer + ipHeaderLength + sizeof(struct udphdr);
            int payloadLength = length - ipHeaderLength - sizeof(struct udphdr);

            int sourcePort = ntohs(udpHeader->source);
            int destPort = ntohs(udpHeader->dest);

            std::cout << "Source Port: " << sourcePort << std::endl;
            std::cout << "Destination Port: " << destPort << std::endl;

            printProtocolDetails(sourcePort, destPort, payload, payloadLength);
        } else {
            std::cout << "Protocol: " << static_cast<int>(ipHeader->protocol) << std::endl;
        }
    }
}

int main() {
    // Create a raw socket
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sockfd < 0) {
        std::cerr << "Socket creation failed!" << std::endl;
        return -1;
    }

    std::string interfaceName;
    std::cout << "Enter a network interface : ";
    std::cin >> interfaceName;
    std::cout << "Listening on interface " << interfaceName << std::endl;

    if (setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, interfaceName.c_str(), interfaceName.size()) < 0) {
        std::cerr << "Failed to bind to interface " << interfaceName << std::endl;
        close(sockfd);
        return -1;
    }
    
    const unsigned int bufferSize = 65536;
    char buffer[bufferSize];

    // Receive packets in a loop
    while (true) {
        sockaddr_in src_addr;
        socklen_t src_addr_len = sizeof(src_addr);
        int bytesReceived = recvfrom(sockfd, buffer, bufferSize, 0, reinterpret_cast<sockaddr*>(&src_addr), &src_addr_len);
        if (bytesReceived < 0) {
            std::cerr << "Error in recvfrom." << std::endl;
            continue;
        }

        // Process the packet (buffer) here
        printPacketInfo(buffer, bytesReceived);
        std::cout << "Packet received!" << std::endl;
    }

    // Close the socket when done
    close(sockfd);
    return 0;
}