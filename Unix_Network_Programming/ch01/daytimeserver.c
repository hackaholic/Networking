/*
   Author: Kumar Shubham
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <error.h>
#include <time.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 100
#define LISTENQ 1024

int main(int argc, char *argv[]) {
    char buf[MAXLINE];
    int sockfd, connfd;
    struct sockaddr_in servaddr, clientaddr;
    time_t now;

    socklen_t clientlen = sizeof(clientaddr);

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error while opening socket\n");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(13);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1 ) {
        perror("Error in binding ....");
        exit(EXIT_FAILURE);
    }

    if(listen(sockfd, LISTENQ) == -1){
        perror("Error while Listen ...");
        exit(EXIT_FAILURE);
    }
    
    for(;;) {
        /*
          When you use the accept function to accept a new connection, you typically pass a pointer to a struct sockaddr variable where the 
          client's address information will be stored. This allows you to access the client's address information, 
          such as their IP address and port number.
        */
        // connfd = accept(sockfd, (struct sockaddr *) NULL, NULL);
        connfd = accept(sockfd, (struct sockaddr *) &clientaddr, &clientlen);
        printf("Got connection from %s, %i\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
        now = time(NULL);
        snprintf(buf, sizeof(buf), "%s\r\n", ctime(&now));
        if(write(connfd, buf, strlen(buf)) == -1) {
            perror("Error while writing data to client");
        }
        close(connfd);
    }

    return 0; 


}
