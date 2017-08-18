#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define PORT "3490"
#define BACKLOG 10
#define MAXDATASIZE 100

int main(){

  int status, sockfd, new_fd, numbytes, i, j;
  struct addrinfo hints, *res, *p;
  char buf[MAXDATASIZE];
  int yes = 1;
  int lengthofmsg;
  char remoteip[INET6_ADDRSTRLEN];
  struct sockaddr_storage their_addr;
  socklen_t addrlen;

  fd_set all;
  fd_set read_fds;
  int fdmax;
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
      continue; 
    }

    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
    
      perror("setsockopt");
      exit(1);
    }
    
    if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1){

      close(sockfd); 
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

  FD_SET(sockfd, &all);
  fdmax = sockfd;

    for(;;){
  
        read_fds = all; //copying all fds to read_fds
        if(select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1){
            perror("select");
            exit(1);
        }// end of if select

        for(i=0; i<=fdmax; i++){
    
            if(FD_ISSET(i, &read_fds)){
       
	            if(i == sockfd){
	                addrlen = sizeof their_addr;
                    new_fd = accept(sockfd, (struct sockaddr*)&their_addr, &addrlen);
  
	                if(new_fd == -1){
                        perror("accept");
                    }else{
                        FD_SET(new_fd, &all); //adding to set of all fds
                        if(new_fd > fdmax){
	                        fdmax = new_fd;
                        }
          
                        printf("server got connection from: %s on socket %d\n", inet_ntop(their_addr.ss_family, &their_addr, remoteip, INET6_ADDRSTRLEN), new_fd);
                    }

                }else{
                    printf("in else\n");          
	                if((numbytes = recv(i, buf, sizeof buf, 0)) <= 0){
	                    if(numbytes == 0) // connection is closed
	                        printf("socket %d closed\n", i);
                        else
                            perror("recv");
                        close(i);
                        FD_CLR(i, &all); // revmoving from set of all fds
                    }else{
                        printf("received: %s\n",buf);
	                    for(j =0;j <= fdmax; j++){
	                        if(FD_ISSET(j, &all)){
		                        if(j != sockfd && j != i){
		                            if(send(j, buf, numbytes, 0) == -1){
 		                                perror("send");
                                    }
                                }
                            }
                        } 
                    }
                }
            } // end of fd_isset
       } // end for throgh fds
    }// end of infinite for
  return 0;
}
