#include "handle_tcp_connection.h"

void *handle_tcp_connections(void *arg) {
    int tcp_socket = *((int *)arg);
    int client_socket;
    struct sockaddr_in client_addr;

    if (listen(tcp_socket, 1) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        socklen_t client_addr_len = sizeof(client_addr);
        if ((client_socket = accept(tcp_socket, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        char buffer[MAX_BUF_SIZE];
        ssize_t bytes_received;

        bytes_received = recv(client_socket, buffer, MAX_BUF_SIZE, 0);
        if (bytes_received == -1) {
            perror("Receive failed");
            exit(EXIT_FAILURE);
        } else if (bytes_received == 0) {
            printf("Client disconnected.\n");
            close(client_socket);
            continue;
        }

        buffer[bytes_received] = '\0';
        printf("Received from client: %s\n", buffer);

        send(client_socket, "server ok", strlen("server ok"), 0);

        close(client_socket);
    }
}
