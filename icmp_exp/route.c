#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

#define BUFSIZE 512

enum{ON, OFF};
enum{PATH, DST_IP};

struct packet_udp {
    struct ip *ip;
    struct udphdr udp;
};

void make_ip_header(struct ip *ip, int srcip, int dstip, int iplen);
void make_udp_header(struct udphdr *udp);
void tvsub(struct timeval *out, struct timeval *in);

int main(int argc, char const *argv[])
{
    struct packet_udp sendpacket;
    struct sockaddr_in send_sa;
    int send_sd;
    int recv_sd;
    int len;
    int ttl;
    u_char buf[BUFSIZE];
    struct icmp *icmp;
    struct ip *ip;
    struct in_addr ipaddr;
    int on = 1;
    int dns_flag = ON;

    memset(&send_sa, 0, sizeof(send_sa));
    send_sa.sin_family = AF_INET;
    send_sa.sin_addr.s_addr = inet_addr(argv[DST_IP]);
    if(send_sa.sin_addr.s_addr == INADDR_NONE){
        send_sa.sin_addr.s_addr = gethostbyname(argv[DST_IP])->h_addr;
    }

    send_sd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);

    setsockopt(send_sd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on));

    recv_sd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    len = sizeof(struct packet_udp);
    memset(&sendpacket, 0, sizeof(sendpacket));
    make_udp_header(&(sendpacket.udp));
    make_ip_header(&(sendpacket.ip), 0, send_sa.sin_addr.s_addr, len);

    fprintf(stderr, "scanroute %s\n", inetntoa(send_sa.sin_addr));
    for(ttl = 1; ttl <= 64; ++ttl){
        struct timeval tvm0;
        struct timeval tvm1;
        struct timeval tv;
        int i;

        fprintf(stderr, "%2d:", ttl);
        sendpacket.ip->ip_ttl = ttl;

        for ( i = 0; i < 3; i++)
        {
            sendto(send_sd, (char *)&sendpacket, len,0, (struct sockaddr *)&send_sa, sizeof(send_sa));
            gettimeofday(&tvm0, 0);
            tv.tv_sec = 3;
            tv.tv_usec = 0;

            for(;;){
                fd_set readfd;
                int n;
                int ihlen;

                FD_ZERO(&readfd);
                FD_SET(recv_sd, &readfd);
                n = select(recv_sd + 1, &readfd, NULL, NULL, &tv);
                if(n == 0){
                    fprintf(stderr, "? :");
                    break;
                }
                recvfrom(recv_sd, buf, BUFSIZE, 0, NULL, NULL);

                gettimeofday(&tvm1, 0);

                ip = (struct ip *) buf;
                ihlen = ip->ip_hl << 2;
                icmp = (struct icmp *) (buf + ihlen);
                if((icmp->icmp_type == ICMP_TIMXCEED && icmp->icmp_code == ICMP_TIMXCEED_INTRANS) || icmp->icmp_type == ICMP_UNREACH) goto exit_loop;
            }
        }
        exit_loop:
        if(i < 3){
            char hostip[INET_ADDRSTRLEN];

            memcpy(&ipaddr, &ip->ip_src.s_addr, sizeof ipaddr);
            snprintf(hostip, INET_ADDRSTRLEN, "%s", inet_ntoa(*(struct in_addr *) &(ip->ip_src.s_addr)));

            if(){

            }else{

            }
            
        }
    }
    end_program:
    close(send_sd);
    close(recv_sd);
    return 0;
}



void make_ip_header(struct ip *ip, int srcip, int dstip, int iplen){
    memset(ip, 0, sizeof(*ip));

    ip->ip_v = IPVERSION;
    ip->ip_hl = sizeof(struct ip);
    ip->ip_id = htons(0);
    ip->ip_off = 0;
    
    // using linux
    ip->ip_len = htons(iplen);
    ip->ip_off = htons(0);

    ip->ip_ttl = 64;
    ip->ip_p = IPPROTO_UDP;
    ip->ip_src.s_addr = srcip;
    ip->ip_dst.s_addr = dstip;

    // checksum
    ip->ip_sum = 0;
}
void make_udp_header(struct udphdr *udp){
    udp->uh_sport = htons(33434);
    udp->uh_ulen  = htons((u_int16_t) sizeof(struct udphdr));
    udp->uh_dport = htons(33434);
    udp->uh_sum   = htons(0);
}
void tvsub(struct timeval *out, struct timeval *in){
    if((out->tv_usec -= in->tv_usec) < 0){
        --out->tv_sec;
        out->tv_usec += 1000000;
    }
    out->tv_sec -= in->tv_sec;
}
