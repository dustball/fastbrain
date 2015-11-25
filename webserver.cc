#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pwd.h>
#include <grp.h>


void sigchld_handler(int s) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}


class webserver {

    protected:

    int sockfd, new_fd;
    struct sockaddr_in their_addr;
    socklen_t sin_size;
    struct sigaction sa;

    void create_and_bind() {
        int level = 1;
        struct sockaddr_in my_addr;

        if  ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
            perror("socket");
            exit(1);
        }
        if  (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &level, sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }
            my_addr.sin_family = AF_INET;
        my_addr.sin_port = htons(8082);
        my_addr.sin_addr.s_addr = INADDR_ANY;
        memset(&(my_addr.sin_zero), '\0', 8);

        if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
            perror("bind");
            exit(1);
        }
        if (listen(sockfd, 4) == -1) {
            perror("listen");
            exit(1);
        }
        sa.sa_handler = sigchld_handler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        if (sigaction(SIGCHLD, &sa, NULL) == -1) {
            perror("sigaction");
            exit(1);
        }
    }

    char in[3000], sent[500], length[20];
    char *result, *lines;
    int buffer_chunks;
    long filesize;

    public:

    void start_server() {
        result = NULL;
        create_and_bind();
    }

    unsigned long int process_request() {

        while (1) {

            sin_size = sizeof(struct sockaddr_in);
            if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) {
                perror("accept");
                exit(1);
            }
            if (!fork()) {
                //close(sockfd);
                if (read(new_fd, in, 3000) == -1) {
                    perror("recive");
                    exit(1);
                } else {

                    //printf("LINES: %s\n", in);

                    lines = strtok(in, "\n\r");

                    unsigned long int neuron;
                    char ch;
                    sscanf(lines, "GET /%c/%lu", &ch, &neuron);

                    sprintf(length, "%d\n", 2);

                    if (ch == 'n') {
                        strcpy(sent, "HTTP/1.1  \n");
                        strcat(sent, "200 OK");
                        strcat(sent, "\nServer: fastbrain 0.1\n");
                        strcat(sent, length);
                        strcat(sent, "Connection: close\nContent-Type: ");
                        strcat(sent, "text/html");
                        strcat(sent, "; charset=");
                        strcat(sent, "UTF-8");
                        strcat(sent, "\n\nOK");
                        write(new_fd, sent, strlen(sent));
                        close(new_fd);
                        return neuron;
                    } else {
                        strcpy(sent, "HTTP/1.1  \n");
                        strcat(sent, "404 Not Found");
                        strcat(sent, "\nServer: fastbrain 0.1\n");
                        strcat(sent, "\nConnection: close\n");
                        close(new_fd);
                        continue;
                    }


                }

            }
            close(new_fd);
        }

    }

};
