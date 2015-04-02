#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define PORT        10002
#define BUFFER_SIZE 1024

#define PASSWORD "hEll0!!"
#define SECRET "2zcccmtpmzpgfpdhgmdghdfkh"


int matches(char *str1, char *str2, int index) {
    usleep(100);

    if (str1[index] != str2[index]) {
        return 0;
    }

    if (str1[index] == 0) {
        return 1;
    }

    return matches(str1, str2, index + 1);
}


int main(int argc, char argv[]) {
    char str[BUFFER_SIZE];
    int listen_fd, comm_fd;
    int i;

    struct sockaddr_in servaddr;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (!listen_fd) {
        perror("couldn't create socket");
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if (bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr))) {
        perror("couldn't bind socket");
        return -1;
    }

    if (listen(listen_fd, 10)) {
        perror("couldn't listen on socket");
        return -1;
    }

    while (1) {
        comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);
        memset(str, 0, BUFFER_SIZE);

        recv(comm_fd, str, BUFFER_SIZE, 0);
        for (i = 0; i < BUFFER_SIZE && str[i]; ++i) {
            if (str[i] == '\n' || str[i] == '\r') {
                str[i] = 0;
                break;
            }
        }

        str[BUFFER_SIZE - 1] = 0;
        if (matches(str, PASSWORD, 0)) {
            send(comm_fd, SECRET, strlen(SECRET), 0);
            send(comm_fd, "\n", 1, 0);
        } else {
            send(comm_fd, "denied\n", strlen("denied\n"), 0);
        }

        close(comm_fd);
    }
    return 0;
}
