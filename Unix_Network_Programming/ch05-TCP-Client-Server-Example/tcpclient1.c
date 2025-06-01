/*
  Author: kumar shubham
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <error.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLEN 1000
#define ECHO_Port 13

void str_cli(FILE *fp, int sockfd) {
  char buff[MAXLEN];
  int n;
  while(fgets(buff, MAXLEN, fp)  !=NULL) {
    write(sockfd, buff, strlen(buff));

    if((n = read(sockfd, buff, MAXLEN)) == 0) {
      perror("str_cli: Server Terminated prematurely\n");
      exit(EXIT_FAILURE);
    }

    if(n > 0) {
      printf("From server: ");
      fwrite(buff, 1, strlen(buff), stdout);
    }
  }  
}

int main(int argc, char *argv[]){

  int sockfd;
  struct sockaddr_in server_info;
  socklen_t clientlen = sizeof(server_info);
  // Iniatializing sockaddr_info to 0
  memset(&server_info, 0, sizeof(server_info));
  // define Address family IPV4
  server_info.sin_family = AF_INET;
  // define interface, here it is localhost
  inet_pton(AF_INET, "127.0.0.1", &server_info.sin_addr);
  // define port to listen
  server_info.sin_port = htons(13);

  // open socket
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("Socket open error\n");
    exit(EXIT_FAILURE);
  }
  connect(sockfd, (struct sockaddr *) &server_info, clientlen);

  str_cli(stdin, sockfd);
  
  close(sockfd);
  return 0;
}

