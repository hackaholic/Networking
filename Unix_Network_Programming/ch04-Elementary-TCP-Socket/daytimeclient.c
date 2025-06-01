/*
    Author: Kumar Shubham
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>    // socket
#include <sys/types.h>
#include <netinet/in.h>    // sockaddr_in
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>     // inet_pton
#include <time.h>

/*
   sudo tcpdump -nvi lo tcp 'dst port 13 or src port 13'

   To get payload:
   sudo tcpdump -X -nvi lo tcp 'dst port 13 or src port 13'

*/

#define MAXLINE 100

int main(int argc, char *argv[]){
    int sockfd, n;
    char buf[MAXLINE];
    struct sockaddr_in serveradd, clientaddr;
    socklen_t clientlen;

    if(argc != 2) {
        printf("Usage: ./a,out ip_addr");
        exit(0);
    }

    // set 0 to all bit in serveradd
    memset(&serveradd, 0, sizeof(struct sockaddr_in));

    serveradd.sin_family = AF_INET;
    serveradd.sin_port = htons(13);

    if((inet_pton(AF_INET, argv[1], &serveradd.sin_addr)) == -1) {
        perror("Error while converting iP to numeric form");
        exit(1);
    }
    printf("Before socket creation ");
    getchar();
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("Error in creating socket\n");
        exit(EXIT_FAILURE);
    }
    printf("Before connect ");
    getchar();
    if((connect(sockfd, (struct sockaddr *) &serveradd, sizeof(struct sockaddr_in)) == -1)){
        perror("Error While connecting to server");
        exit(2);
    }
    printf("After connect ");
    getchar();
    clientlen = sizeof(clientaddr);    
    getsockname(sockfd, (struct sockaddr *) &clientaddr, &clientlen);

    printf("Ephemeral port opened by kernel %i\n", ntohs(clientaddr.sin_port));

    

    printf("Before reading from socket ");
    getchar();
    while((n = read(sockfd, buf, MAXLINE)) > 0 ) {
        buf[n] = 0;
    }

    if (n<0){
        perror("Read error");
        exit(3);
    }
    printf("Server reply: %s\n", buf);

    close(sockfd);
    return 0;
}