
#include <pthread.h>
#include "broadcast_udp.h"
#include "handle_tcp_connection.h"

int main() {
    int udp_socket, tcp_socket;
    struct sockaddr_in udp_server_addr, tcp_server_addr;

    // Create UDP socket
    if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("UDP socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&udp_server_addr, 0, sizeof(udp_server_addr));
    udp_server_addr.sin_family = AF_INET;
    udp_server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    udp_server_addr.sin_port = htons(UDP_PORT);

    // Bind UDP socket
    if (bind(udp_socket, (struct sockaddr*)&udp_server_addr, sizeof(udp_server_addr)) == -1) {
        perror("UDP bind failed");
        exit(EXIT_FAILURE);
    }

    // Create TCP socket
    if ((tcp_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("TCP socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&tcp_server_addr, 0, sizeof(tcp_server_addr));
    tcp_server_addr.sin_family = AF_INET;
    tcp_server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    tcp_server_addr.sin_port = htons(TCP_PORT);

    // Bind TCP socket
    if (bind(tcp_socket, (struct sockaddr*)&tcp_server_addr, sizeof(tcp_server_addr)) == -1) {
        perror("TCP bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server started.\n");

    // Create threads for UDP broadcasting and TCP connection handling
    pthread_t udp_thread, tcp_thread;
    if (pthread_create(&udp_thread, NULL, broadcast_udp, &udp_socket) != 0) {
        perror("Thread creation for UDP broadcast failed");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&tcp_thread, NULL, handle_tcp_connections, &tcp_socket) != 0) {
        perror("Thread creation for TCP connection handling failed");
        exit(EXIT_FAILURE);
    }

    // Join threads
    pthread_join(udp_thread, NULL);
    pthread_join(tcp_thread, NULL);

    // Close sockets (not reachable due to infinite loops in threads)
    close(udp_socket);
    close(tcp_socket);

    return 0;
}

