#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 5000
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr *)&server_addr,
                sizeof(server_addr)) < 0) {
        perror("Connect failed");
        exit(1);
    }

    /* ---- Send data ---- */
    char message[] = "Hello from client";
    write(sockfd, message, strlen(message));

    /* ---- Receive server reply ---- */
    n = read(sockfd, buffer, BUFFER_SIZE);
    buffer[n] = '\0';
    printf("Server replied: %s\n", buffer);

    /* ---- SHUTDOWN WRITE SIDE ---- */
    printf("Client calling shutdown(SHUT_WR)\n");
    shutdown(sockfd, SHUT_WR);

    /* ---- First recv after shutdown ---- */
    n = read(sockfd, buffer, BUFFER_SIZE);
    printf("First read after shutdown returned: %d\n", n);

    /* ---- Second recv: should return 0 ---- */
    n = read(sockfd, buffer, BUFFER_SIZE);
    printf("Second read after shutdown returned: %d (FIN received)\n", n);

    close(sockfd);
    return 0;
}
