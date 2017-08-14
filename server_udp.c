
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>

#define PORT "4567"
#define MAXBUFLEN 100


int main(){

  int sockfd, newfd, status, numbytes;
  struct addrinfo hints, *res, *p;
  struct sockaddr_storage their_addr;
  char in_buf[MAXBUFLEN], out_buf[MAXBUFLEN];
  char s[INET6_ADDRSTRLEN];
  socklen_t addr_len;
  struct hostent *hostp;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  if((status = getaddrinfo(NULL, PORT, &hints, &res)) != 0){
  
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    exit(1);
  }
  
  for(p = res; p != NULL; p = p->ai_next){

    if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
     
      perror("socket");
      continue;
    }
    if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1){
      close(sockfd);
      perror("bind");
      continue;
    }
    break;
  }
  if(p == NULL){
    
    fprintf(stderr,"sever failed to bind socket\n");
    exit(2);
  }
  freeaddrinfo(res);
  
    addr_len = sizeof their_addr;
    while(1){
    if((numbytes = recvfrom(sockfd, in_buf, MAXBUFLEN-1, 0, (struct sockaddr*)&their_addr, &addr_len)) == -1){
      perror("recvfrom");
      exit(1);
    }
    inet_ntop(their_addr.ss_family, &their_addr, s, sizeof s);
    in_buf[numbytes] = '\0';  
    printf("Message from %s:%s\n", s, in_buf);

    if((numbytes = sendto(sockfd, in_buf, strlen(in_buf), 0, (struct sockaddr*)&their_addr, addr_len)) == 1){
  
    perror("sendto");
    exit(1);
   }
    }
 close(sockfd);
 return 0;
}



