#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <unistd.h>

int main() {
    // Create a raw socket
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sockfd < 0) {
        std::cerr << "Socket creation failed!" << std::endl;
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
        // ...

        std::cout << buffer << std::endl;
    }

    // Close the socket when done
    close(sockfd);
    return 0;
}