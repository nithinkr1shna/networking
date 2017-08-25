// Standard forking http server

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define DEFAULT_PORT "3419" 	
#define PORT_SIZE 6
#define MAX_CLIENT_CONN 200
#define CLIENT_DATA 9999
#define SEND_SIZE 1024
#define BAD_REQ "HTTP/1.0 400 Bad Request\n"
#define OK "HTTP/1.0 200 OK\n\n"
#define NOT_FOUND "HTTP/1.0 404 Not Found\n"
#define PATH_MAX_SIZE 9999

char *root;
char clients[MAX_CLIENT_CONN];
int listen_fd; //fd responsible for listening
// need to set default port 
// IP is 127.0.0.1 
// Port as a cmd line argument
// working directory is the root of the server
void start_server(char *);
void respond_to_client(int);
void print_i(int );
void print_s(char *str);

// main function

int main(int argc, char **argv){

	char port[PORT_SIZE];
    int i, each_client = 0;
    struct sockaddr_in clientaddr;
    socklen_t addrlen;
    pid_t child;

    root = getenv("PWD");
    print_i(argc);
    print_s(root);
    if(argc <2){
    	print_s("using default port: 3419\n");
    	strcpy(port,DEFAULT_PORT);
    }else{

    	print_s("using port :");
    	print_s(argv[1]);
    	strcpy(port, argv[1]);
    }

    for(i =0 ;i<MAX_CLIENT_CONN;i++){

    	clients[i] = -1; // indicates that no client is connected 
    }

    start_server(port); // server is started

    // have to accpte connections from client    

    for(;;){

       addrlen = sizeof clientaddr;
       clients[each_client] = accept(listen_fd, (struct sockaddr*)&clientaddr, &addrlen);

       if(clients[each_client] < 0){
           //implies that no successful accept 
       	   perror("error in accept()");
       }else{
           // we hava a client
       	   // we have to fork for each client
       	   printf("we hava client\n");
       	   if(fork() == 0){ // successfull fork
              
              respond_to_client(each_client);
              exit(0); // close the connection , dont keep-alive
       	   }

       }
       while(clients[each_client] != -1)
       	  each_client = (each_client+1)%MAX_CLIENT_CONN;
       	  // why ?

    }

    
    return 0;
}

//strating the server at the specified port with loopback IP
void start_server(char *port){

	struct addrinfo hints, *res, *p;
    int status;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if((status = getaddrinfo(NULL, port, &hints, &res)) !=0){

    	fprintf(stderr, "getaddrinfo: %s\n",gai_strerror(status));
    	exit(1);
    }

    for(p= res; p != NULL;  p = p->ai_next){

          if((listen_fd =socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){

          	perror("listen_fd");
          	continue;
          }

          if(bind(listen_fd, p->ai_addr, p->ai_addrlen) == 0) // binding to the socket
          	break;
    }
    if(p == NULL){

    	perror("in socket() or bind()");
    	exit(2);
    }
    freeaddrinfo(res); // freeing memory

    if(listen(listen_fd, 250) != 0){

    	perror("error in listen()");
    	exit(3);
    }

}

//responding to the client

void respond_to_client(int client){

  char data[CLIENT_DATA], path[PATH_MAX_SIZE], to_send[SEND_SIZE];
  char *requests[3], response[SEND_SIZE];
	int received, fd, num_bytes;

	memset((void*)data, (int)'\0',CLIENT_DATA);

	if((received = recv(clients[client], data, sizeof data, 0)) < 0){

		fprintf(stderr, "received %s\n", gai_strerror(received));
	}else if(received == 0){  

        fprintf(stderr, "client disconnected unexpectedly%s\n",gai_strerror(received));

	}else{ // recieved msg from client
        
        printf("%s\n", data); // This is what the client sends
        requests[0] = strtok(data, " \t");
        printf("req0: %s\n", requests[0]);
        if(strncmp(requests[0],"GET\0",4) == 0){ // checking for GET verb
        	
        	  printf("hei");
            requests[1] = strtok(NULL, " \t");
            requests[2] = strtok(NULL, " \t\n");
            printf("req1: %s\n", requests[1]);
            printf("req2: %s\n", requests[2]);
            if(strncmp(requests[2],"HTTP/1.1",8) != 0 && strncmp(requests[2], "HTTP/1.0",8) !=0){

            	//bad request
            	printf("bad req\n");
            	write(clients[client], BAD_REQ, sizeof BAD_REQ);
            }else{
            	//printf("cjeck");

            	if(strcmp(requests[1],"/") == 0){ //no file is specified
            		printf("index.html\n");
            		requests[1] ="/index.html";
                
            	}else{
                printf("have something");
                
              }
            	
              strcpy(path, root);
              strcat(path, requests[1]);
              printf("path: %s\n", path);

              //open the file and send it to client
              if((fd = open(path,O_RDONLY)) != -1){

                send(clients[client], OK, strlen(OK), 0);
                while((num_bytes = read(fd, to_send, SEND_SIZE)) > 0)
                  write(clients[client], to_send, num_bytes);
                //close(fd);
              }else{

                write(clients[client], NOT_FOUND, sizeof NOT_FOUND);
              }


            }
        }

	}

  shutdown(clients[client], SHUT_RDWR);
  close(clients[client]);
  clients[client] = -1;


}

//helper functions

void print_i(int a){

	printf("%d\n",a);
}

void print_s(char *str){

	printf("%s", str);
}