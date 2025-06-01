/*
  Author: kumar shubham
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <error.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLEN 1000
#define ECHO_Port 13

static int sockfd;

void sig_handler(int sig, siginfo_t *info, void *ucontext) {
    switch (sig)
    {
    case SIGINT:
      printf("caught CTRL-C\n");
      sleep(3);
      close(sockfd);
      break;
    default:
        break;
    }
}

void str_cli(FILE *fp, int sockfd) {
  char sendbuff[MAXLEN], recvbuff[MAXLEN];
  int n;
  fd_set  rset;

  FD_ZERO(&rset);

  for (;;) {
    // set file discriptor to minitor

    FD_SET(STDIN_FILENO, &rset);
    FD_SET(sockfd, &rset);
    if(select(sockfd+1, &rset, NULL, NULL, NULL)< 0 ) {
      perror("str_cli: select error");
      exit(EXIT_FAILURE);
    }

    // check is stdin is ready
    if(FD_ISSET(STDIN_FILENO, &rset)){
      if(fgets(sendbuff, MAXLEN, fp) != NULL) {
         write(sockfd, sendbuff, strlen(sendbuff));
      }
    }

    // check if socket is ready for read
    if(FD_ISSET(sockfd, &rset)){
      if((n = read(sockfd, recvbuff, MAXLEN)) == 0) {
        perror("str_cli: Server Terminated prematurely\n");
        exit(EXIT_FAILURE);
      }

      if(n > 0) {
        printf("From server: ");
        fwrite(recvbuff, 1, strlen(recvbuff), stdout);
      }
    }
  }
}

int main(int argc, char *argv[]){
  struct sockaddr_in server_info;
  struct sigaction sa;
  socklen_t clientlen = sizeof(server_info);

  // Capture Signal
  sa.sa_handler = &sig_handler;
  sa.sa_flags = SA_SIGINFO;
  sigemptyset(&sa.sa_mask);

  if(sigaction(SIGINT, &sa, NULL) == -1) {
      perror("Error: sigaction");
      exit(EXIT_FAILURE);
    }


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

