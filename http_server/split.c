#include <stdio.h>
#include <stdlib.h>

struct hd{

  char head[100];
  char body[9999];
};

struct hd split(char *value){
  char *p;
  p =value;
  struct hd hd_and_msg;
  //char hd[100];
  //char body[9999];
  int i =0, flag =0, j=0;
  char *pointer[2];
  while(*p != NULL){

    //printf("%c", *p);
     if(*p == '\n'){
      flag =1;
      
    }
    if( flag == 0){

      hd_and_msg.head[i] = *p;
      i++;
    }
   
    if(flag == 1 && *p != '\n'){

      hd_and_msg.body[j] = *p;
      j++;
    }

    *p++;
  }
 
  
  return hd_and_msg;

}

/*
int main(){

  //char *p;
  char* value = "Content-Type:text/plain\n\nfactorial is 345";
  struct hd p;
  //char* header_msg
  p=  split(value);
  printf("%s\n",p.head);
  printf("Body:%s\n", p.body);
 
}
*/
