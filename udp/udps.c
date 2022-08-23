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

typedef struct sockaddr_in sockaddr_in;
typedef struct servent servent;
typedef struct sockaddr sockaddr;

enum {PATH, DST_PORT};

int main(int argc, char const *argv[])
{
    sockaddr_in server;
    sockaddr_in client;
    int len;
    int port;
    int s;
    int cn;
    int rn;
    int size;
    char recv_buf[BUFSIZE];
    char send_buf[BUFSIZE];
    char cmd1[MSGSIZE], cmd2[MSGSIZE];

    if(argc == 2){
        if((port = atoi(argv[DST_PORT]) == 0)){
            servent *se;

            if((se = getservbyname(argv[DST_PORT], "udp")) != NULL){
                port = (int) ntohs((u_int16_t) se->s_port);
            } else{
                fprintf(stderr, "getservbyname error");
                exit(EXIT_FAILURE);
            }
        } 
    }

    if((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server, 0, sizeof server);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);

    if(bind(s, (sockaddr *) &server, sizeof server) < 0){
        perror("bind");
        exit(EXIT_FAILURE);
    }

    while (rn = recvfrom(s, recv_buf, BUFSIZE - 1, 0, (sockaddr *) &client, (len = sizeof client, &len)) >= 0)
    {
        recv_buf[rn] = '\n';
        fprintf(stderr, "recieve from -> %s\n", inet_ntoa(client.sin_addr));
        fprintf(stderr, "recieve data -> %s\n", recv_buf);

        if((cn = sscanf(recv_buf, "%s%s", cmd1, cmd2)) <= 0){

        }else{
            if(strcmp(cmd1, "quit") == 0){
                break;
            }
        }

        size = 3;
        strncpy(send_buf, recv_buf, 3);
        if(sendto(s, send_buf, size, 0, (sockaddr *) &client, len) <  0){
            perror("sendto");
            exit(EXIT_FAILURE);
        }
    }
    close(s);
    
    return 0;
}
