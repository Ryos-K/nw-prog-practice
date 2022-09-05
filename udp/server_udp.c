#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 32768
#define MSGSIZE 1024
#define DEFAULT_PORT 5320

typedef struct sockaddr_in sockaddr_in;
typedef struct servent servent;
typedef struct sockaddr sockaddr;

enum {PATH, DST_PORT};

int main(int argc, char const *argv[])
{
    int port;
    int s_fd;
    int rn, len;
    char buf[BUFSIZE];
    sockaddr_in server, client;
    port = DEFAULT_PORT;
    s_fd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&server, 0, sizeof server);
    server.sin_family = AF_INET;
    server.sin_port =  htons(port);
    server.sin_addr.s_addr =  htonl(INADDR_ANY);
    bind(s_fd, (sockaddr *) &server, sizeof server);
    while (1)
    {
        rn = recvfrom(s_fd, buf, BUFSIZE - 1, 0, (sockaddr *) &client, (len = sizeof client, &len));
        buf[rn] = '\0';
        printf("receive from %s\n", inet_ntoa(client.sin_addr));
        printf("receive data %s:%d\n", buf, rn);
        if(strcmp(buf, "quit") == 0) break;
    }
    fprintf(stderr, "bye!\n");
    close(s_fd);
    return 0;
}
