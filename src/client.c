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
#include <arpa/inet.h>

#define MAXKEYSIZE 260
#define MAXVALUESIZE 262150
#define MAXBUFSIZE 264000

char *toXML(char buffer[])
{
  char delim[] = " ";
  char msgType[10] = {0};
  char key[MAXKEYSIZE] = {0};
  char value[MAXVALUESIZE] = {0};
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
    // printf("INvalid");
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
  char buffer[MAXBUFSIZE]="";
  char key[MAXKEYSIZE]={0}, value[MAXVALUESIZE]={0}, msg[50]={0}, msgType[10]={0};
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

  printf("RESPONSE:\t%s\n",buffer);
  fprintf(optr,"%s\n", buffer);

}

void main(int argc, char *argv[])
{
  
  char msg[50];
  struct sockaddr_in addr = {0};
  int n, sockfd,num=1;
   char resp[MAXBUFSIZE];
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
  char buffer[MAXBUFSIZE];
  // printf("%d\n", argc);
  if(argc > 1)
  {

  if(argc == 3)
  {
    strcpy(server_ip, argv[1]);
//  if(argc >= 3)
    port = atoi(argv[2]);
   // printf("yess\n");
  }
  else if(argc == 5)
  {
    // if(argc >= 4)
      strcpy(inputfile, argv[3]);
    // if(argc >= 5)
      strcpy(outputfile, argv[4]);
    }
  }
  
  // printf("server ip %s\n\n" , server_ip);

  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(server_ip);
 
  if(connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
  {
    puts("Network Error: Could not connect");
    exit(1);
  }
 
  printf("child {%d} connected \n", getpid());
 
   
  if(argc != 3)
  {
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
    
    
    while(fgets(buffer, MAXBUFSIZE, iptr) != NULL){
      char *buf = strtok(buffer, "\n");
      printf("SENDING:\t%s \n", buf);
      
      
      buf = toXML(buf);
      // puts(buf);
      write(sockfd, buf, strlen(buf));

      // sleep(2);
      
      int len = read(sockfd, resp, MAXBUFSIZE);
      resp[len] = '\0';
      bufferDump(resp, optr);
      printf("--------------------------------------\n");
      // sleep(3);
    }

    fclose(iptr);
    fclose(optr);
  }
  else
  {
    printf("Interactive mode: case sensitive command PUT, GET, DEL. Please enter \"exit\" to terminate\n");
    while(1)
    {
      printf("IN:\t");
      scanf("%[^\n]%*c", buffer); 
      if(strcmp(buffer , "exit"))
      {
        char *buf = buffer;
        buf = toXML(buf);
        write(sockfd, buf, strlen(buf));
        int len = read(sockfd, resp, MAXBUFSIZE);
        resp[len] = '\0';
        char key[MAXKEYSIZE]={0}, value[MAXVALUESIZE]={0}, msg[50]={0}, msgType[10]={0};
        ParseXML(msg, msgType, key, value, resp);
        if(!strcmp(msg, ""))
        {
          printf("OUT:\tkey: %s\tvalue: %s\n",key,value);
        }
        else
        {
          printf("OUT:\t%s\n",msg);
        }
      }
      else
      {
        exit(0);
      }
    }
  }
}