#include "broadcast_udp.h"

#define TCP_PORT 6000

void *broadcast_udp(void *arg) {
    int udp_socket = *((int *)arg);
    struct sockaddr_in broadcast_addr;
    int broadcast_enable = 1;
    memset(&broadcast_addr, 0, sizeof(broadcast_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(UDP_PORT);
    broadcast_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    if (setsockopt(udp_socket, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) == -1) {
        perror("Setsockopt for broadcast failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        char message[MAX_BUF_SIZE];
        // Construct message
        snprintf(message, MAX_BUF_SIZE, "Hello;ip=%s;p=%d", "192.168.200.128", TCP_PORT);

        // Broadcast message
        sendto(udp_socket, message, strlen(message), 0, (struct sockaddr*)&broadcast_addr, sizeof(broadcast_addr));
        printf("Broadcasted: %s\n", message);

        sleep(3);
    }
}
