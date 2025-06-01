/*
  Author: Kumar Shubham
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char *argv[]) {

    struct in_addr addr_ipv4;
    char ip_addr[] = "192.168.2.3";
    char ip_addr2[INET_ADDRSTRLEN];

    memset(&addr_ipv4, 0, sizeof(struct in_addr));
    // conver Ip presentation to network byte order
    assert(inet_pton(AF_INET, ip_addr, &addr_ipv4)==1);
    printf("%s in network: %i\n", ip_addr, addr_ipv4.s_addr);


    // convert back network byte to Ip presentation using inet_ntoa
    printf("inet_ntoa -> %s\n", inet_ntoa(addr_ipv4));

    // inet_ntop
    //  INET_ADDRSTRLEN is a constant defined in <netinet/in.h> that specifies the maximum length of an IPv4 address string in
    // dotted-decimal notation, including the terminating null byte.
    inet_ntop(AF_INET, &addr_ipv4, ip_addr2, INET_ADDRSTRLEN);
    printf("inet_notp -> %s\n", ip_addr2);

    return 0;
}