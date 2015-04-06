#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <errno.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define PORT        10000
#define BUFFER_SIZE 1024

#define PASSWORD "hEll0!!"
#define SECRET "2zcccmtpmzpgfpdhgmdghdfkh"

#define EXTRA 100000


void signal_handler(int sig) {
    if (sig == SIGTERM) {
        printf("shutting down...\n");
        exit(0);
    }
}


int matches(char *str1, char *str2, int index, int *equal) {
    if (str1[index] != str2[index]) {
        if (str2[index] == 0) {
            return 0;
        }

        return 0;
    }

    (*equal)++;

    if (str1[index] == 0) {
        return 1;
    }

    return matches(str1, str2, index + 1, equal);
}


int main(int argc, char argv[]) {
    char str[BUFFER_SIZE];
    int listen_fd, comm_fd;
    int i;
    int optval = 1;

    struct sockaddr_in servaddr;

    signal(SIGTERM, signal_handler);

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (!listen_fd) {
        perror("couldn't create socket");
        return -1;
    }

    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

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
        int equal = 0;
        clock_t start = clock();
        clock_t end = start;
        int result = matches(str, PASSWORD, 0, &equal);
        while(end < start + equal * 2 * CLOCKS_PER_SEC / 1000) {
            end = clock();
        }

        if (result) {
            send(comm_fd, SECRET, strlen(SECRET), 0);
            send(comm_fd, "\n", 1, 0);
        } else {
            send(comm_fd, "denied\n", strlen("denied\n"), 0);
        }

        close(comm_fd);
    }
    return 0;
}
