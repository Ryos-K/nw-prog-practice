#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 1024
#define MSGSIZE 1024
#define DEFAULT_PORT 5320

typedef struct sockaddr_in sockaddr_in;
typedef struct servent servent;
typedef struct sockaddr sockaddr;
typedef struct hostent hostent;

enum{PATH, DST_IP, DST_PORT};

int main(int argc, char *argv[])
{
    int port;
    int s_fd;
    char buf[BUFSIZE];
    sockaddr_in server;
    port = DEFAULT_PORT;
    server.sin_family = AF_INET;
    server.sin_port =  htons(port);
    server.sin_addr.s_addr =  inet_addr(argv[DST_IP]);
    if(server.sin_addr.s_addr == INADDR_NONE){
        server.sin_addr.s_addr = *(uint32_t*)gethostbyname(argv[DST_IP])->h_addr;
    }
    s_fd = socket(AF_INET, SOCK_DGRAM, 0);

    while(1){
        scanf("%s", buf);
        if(sendto(s_fd, buf, strlen(buf), 0, (sockaddr *) &server, sizeof server) < 0) break;
        fprintf(stderr, "send data %s\n", buf);
        if(strcmp(buf, "quit") == 0) break;
    }
    fprintf(stderr, "bye!\n");
    close(s_fd);
    
}