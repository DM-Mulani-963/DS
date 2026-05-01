#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

int main() {
    int sockfd, fd;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    cout << "socket fd: " << sockfd << endl;

    fd = open("data", O_RDONLY);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    cout << "file fd: " << fd << endl;

    for (;;);

    close(sockfd);
    close(fd);

    return 0;
}