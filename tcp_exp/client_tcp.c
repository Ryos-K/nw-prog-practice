#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DEFAULT_PORT    5320
#define BUF_SIZE        8192

enum{PATH, DST_IP, DST_PORT};

int main(int argc, char *argv[]){
        struct sockaddr_in server;
        int s_fd;
        int port = DEFAULT_PORT;
        int len;
        char buf[BUF_SIZE];
        s_fd = socket(AF_INET, SOCK_STREAM, 0);
        memset(&server, 0, sizeof server);
        server.sin_family       = AF_INET;
        server.sin_addr.s_addr  = inet_addr(argv[DST_IP]);
        server.sin_port         = htons(port);
        if(server.sin_addr.s_addr == INADDR_NONE){
                server.sin_addr.s_addr = *(in_addr_t *)gethostbyname(argv[DST_IP])->h_addr;
        }
        connect(s_fd, (struct sockaddr *) &server, sizeof server);

        for(;;)
        {
                fgets(buf, BUF_SIZE - 2, stdin);
                if(send(s_fd, buf, strlen(buf), 0) <= 0) break;
                fprintf(stderr, "send data %s\n", buf);
                if(strcmp(buf, "quit\n") == 0) break;
        }
        fprintf(stderr, "bye!\n");
        close(s_fd);
        
        
}