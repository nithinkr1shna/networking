#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<netdb.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define PORT "3490"

#define MAXDATASIZE 100

int main(){

  int sockfd, numbytes, lengthofmsg;
  char buf[MAXDATASIZE], msg[MAXDATASIZE];
  struct addrinfo hints, *res, *p;
  int status;
  char s[INET6_ADDRSTRLEN];

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if((status = getaddrinfo("127.0.0.1",PORT, &hints, &res)) != 0){
   
    fprintf(stderr, "getaddrinfo:%s\n",gai_strerror(status));
    exit(1);
  }

  for(p= res; p != NULL; p= p->ai_next){

    if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
     
      perror("client:socket");
      continue;
    }

    if(connect(sockfd, p->ai_addr, p->ai_addrlen)  == -1){
    
      close(sockfd);
      perror("client: connect");
      continue;
    }
    break;
  }

  if(p == NULL){

    fprintf(stderr, "client : failed to connect");
    exit(2);
  }
  inet_ntop(p->ai_family, p->ai_addr, s, sizeof s);
  printf("client: connecting to: %s\n",s);

  freeaddrinfo(res);

  while(1){

    printf("Enter the msg for server\n");
    scanf("%s",msg);
    
    if(send(sockfd, msg,strlen(msg), 0)== -1){
   
      perror("send");
    }
    if((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1){
   
      perror("recv");
      exit(1);
    }
    buf[numbytes] ='\0';
    printf("Client received %s\n", buf);
  }  
  close(sockfd);
  
  return 0;
}


