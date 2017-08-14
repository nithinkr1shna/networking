#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<stdlib.h>

#define PORT "3490"
#define BACKLOG 10
#define MAXDATASIZE 100

int main(){

  int status, sockfd, newfd, numbytes;
  struct addrinfo hints, *res, *p;
  char buf[MAXDATASIZE];
  int yes = 1;
  int lengthofmsg;
  char s[INET6_ADDRSTRLEN];
  struct sockaddr_storage their_addr;
  socklen_t sin_size;
  memset(&hints, 0 , sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  

  if((status = getaddrinfo(NULL, PORT, &hints, &res)) != 0){
  
    perror("");
    exit(1);
  }
  for(p = res; p != NULL; p = p->ai_next){
     
    if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
     
      perror("server:error");
      continue; //why continue ?
    }

    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
    
      perror("setsockopt");
      exit(1);
    }
    
    if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1){

      close(sockfd); // why closed?
      perror("server:bind");
      continue;
    }
    break;
  }
  freeaddrinfo(res);
  
  if(p == NULL){
  
    fprintf(stderr, "server: failed to bind");
    exit(1);
  }
  if(listen(sockfd, BACKLOG) == -1){
    perror("in listen");
    exit(1);
  }



  while(1){

    sin_size = sizeof their_addr;
    if((newfd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1){
    
      perror("accept");
      continue;
    }
    
    inet_ntop(their_addr.ss_family, &their_addr,s, sizeof s);
    printf("server: got connection from %s\n", s);
    while(1){
      if((numbytes =  recv(newfd, buf, MAXDATASIZE-1, 0)) == -1){
        perror("recv");
        exit(1);
      }
      buf[numbytes] = '\0';
      if(numbytes > 0)
        printf("server received %s\n",buf);
      else
        break;
      lengthofmsg = strlen(buf);
      close(sockfd);
      if(send(newfd, buf, lengthofmsg, 0)==-1)
        perror("send");
    }
    close(newfd);
    exit(0);
  }
}
