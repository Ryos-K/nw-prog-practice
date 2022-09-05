#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE         8192
#define DEFAULT_PORT    5320

enum{PATH, SRC_PORT};

int main(int argc, char *argv[]){
        struct sockaddr_in server, client;
        int port = DEFAULT_PORT;
        int s_fd, s0_fd;
        int len;
        s0_fd = socket(AF_INET, SOCK_STREAM, 0);
        memset(&server, 0, sizeof server);
        server.sin_family       = AF_INET;
        server.sin_addr.s_addr  = htonl(INADDR_ANY);
        server.sin_port         = htons(port);
        bind(s0_fd, (struct sockaddr *)&server, sizeof server); 
        listen(s0_fd, 5);
        for(;;)
        {
                s_fd = accept(s0_fd, (struct sockaddr *)&client, (len = sizeof client, &len));
                printf("connected from %s\n", inet_ntoa(client.sin_addr));

                for (;;)
                {
                        int i;
                        char recv_buf[BUFSIZE];
                        recv_buf[0] = '\0';
                        for (i = 0; i < BUFSIZE - 1; ++i)
                        {
                                if(recv(s_fd, &recv_buf[i], 1, 0) <= 0) goto exit_loop;
                                if(recv_buf[i] == '\n') break;
                        }
                        recv_buf[i] = '\0';
                        fprintf(stderr, "receive %s\n", recv_buf);

                        if(strcmp(recv_buf, "quit") == 0) goto exit_loop;

                }
                exit_loop:       
                fprintf(stderr, "connection closed\n");
                close(s_fd);
        }
        fprintf(stderr, "bye!\n");
        close(s0_fd);
        return 0;
}