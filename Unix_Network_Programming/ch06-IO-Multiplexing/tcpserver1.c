/*
  Author: Kumar Shubham
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <error.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLEN 1000


static int sockfd;

void sig_handler(int sig, siginfo_t *info, void *ucontext) {
    switch (sig)
    {
    case SIGINT:
      printf("caught CTRL-C\n");
      sleep(3);
      close(sockfd);
      exit(0);
      break;
    default:
        break;
    }
}


void str_echo(int sockfd) {
    char buff[MAXLEN];
    int n;
    while((n = read(sockfd, buff, MAXLEN)) > 0) {
        write(sockfd, buff, n);
    }
}

int main(int argc, char *argv[]){
  
  int sockfd, clientfd;
  struct sockaddr_in server_info, client_info;
  struct sigaction sa;

  socklen_t serverlen , clientlen;
  serverlen = sizeof(server_info);
  clientlen = sizeof(client_info);

  // Capture Signal
  sa.sa_handler = sig_handler;
  sa.sa_flags = SA_SIGINFO;
  sigemptyset(&sa.sa_mask);

  if(sigaction(SIGINT, &sa, NULL) == -1) {
      perror("Error: sigaction");
      exit(EXIT_FAILURE);
    }

  memset(&server_info, 0, serverlen);
  memset(&client_info, 0, clientlen);
  server_info.sin_family = AF_INET;
  server_info.sin_port = htons(13);
  inet_pton(AF_INET, "127.0.0.1", &server_info.sin_addr);

  // Open socket
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {
    perror("Socket open error\n");
  } 

  if(bind(sockfd, (struct sockaddr *) &server_info, serverlen) == -1) {
    perror("Error while bind\n");
  }

  listen(sockfd, 5);

  for(;;)
  {
    if((clientfd = accept(sockfd, (struct sockaddr *) &client_info, &clientlen)) == -1 ) {
      perror("Error: accept....");
    }
    str_echo(clientfd);
  }
  
  close(sockfd);
  return 0;
}