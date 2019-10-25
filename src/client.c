#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <unistd.h>
#include "xmlParser.h"


#define MAXBUF 300000

char *toXML(char buffer[]){
  char delim[] = " ";
  char msgType[10] = {0};
  char key[260] = {0};
  char value[256*1024 + 10] = {0};
  char *ptr = strtok(buffer, delim);
  

  if(!strcmp(ptr, "PUT"))
  {
    strcpy(msgType, "putreq");
  }
  else if(!strcmp(ptr, "GET"))
  {
    strcpy(msgType, "getreq");
  }
  else if(!strcmp(ptr, "DEL"))
  {
    strcpy(msgType, "delreq");
  }
  else
  {
    printf("error in parsing");
    // TODO specify error types
  }

  ptr = strtok(NULL, delim);
  strcpy(key, ptr);
  ptr = strtok(NULL, delim);
  if(ptr)
    strcpy(value, ptr);
  else
    strcpy(value, "");

  char *response = GenerateXML(key, value, "", msgType);
  
  
  response[strlen(response)] = '\0';

  return response;
}

//WRITING RESPONSE IN THE FILE
void bufferDump(char resp[], FILE *optr)
{
  char buffer[300000]="";
  char key[256]={0}, value[256*1024]={0}, msg[50]={0}, msgType[10]={0};
  ParseXML(msg, msgType, key, value, resp);

  // strcat(buffer, msgType);
  // strcat(buffer, " ");    

  if(!strcmp(msg, ""))
  {
    strcat(buffer, key);
    strcat(buffer, " ");
    strcat(buffer, value);
  }
  else
  {
    strcat(buffer, msg);
  }

  //strcat(buffer, "\0");

  printf("%s\n",buffer);
  fprintf(optr,"%s\n", buffer);

}

void main(int argc, char *argv[])
{
  
  char msg[MAXBUF];
  struct sockaddr_in addr = {0};
  int n, sockfd,num=1;
  // srandom(getpid());
  /* Create socket and connect to server */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if(sockfd < 0)
  {
    puts("Network Error: Could not create socket");
    exit(1);
  }

  int port = 8080;
  char server_ip[20] = "127.0.0.1";
  char inputfile[100] = "ClientData/batchRequest.txt";
  char outputfile[100] = "ClientData/batchResponse.txt";

  

  if(argc >= 2)
    strcpy(server_ip, argv[1]);
  if(argc >= 3)
    port = atoi(argv[2]);
  if(argc >= 4)
    strcpy(inputfile, argv[3]);
  if(argc >= 5)
    strcpy(outputfile, argv[4]);
  

  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(server_ip);
 
  if(connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
  {
    puts("Network Error: Could not connect");
    exit(1);
  }
 
  printf("child {%d} connected \n", getpid());
  FILE *iptr;
  FILE *optr;
   
  

  if ((iptr = fopen(inputfile, "r")) == NULL)
  {
      printf("Error! opening file");
      // Program exits if file pointer returns NULL.
      exit(1);         
  }
  // sprintf(filename, "ClientData/clientResponses_%d.txt",getpid() );
  if ((optr = fopen(outputfile, "a")) == NULL)
  {
      printf("Error! opening file");
      // Program exits if file pointer returns NULL.
      exit(1);         
  }
  char buffer[300000];
  
  while(fgets(buffer, 300000, iptr) != NULL){
    char *buf = strtok(buffer, "\n");
    printf("SENDING: %ld bytes\n", strlen(buf));
    printf("===\n");
    
    buf = toXML(buf);
    // puts(buf);
    write(sockfd, buf, strlen(buf));

    // sleep(2);
    char resp[300000];
    int len = read(sockfd, resp, 300000);
    resp[len] = '\0';
    // puts(resp);

    bufferDump(resp, optr);

    // sleep(3);
    // fclose(optr);
  }
  

  
  fclose(iptr);
  fclose(optr);
}