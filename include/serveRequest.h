#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>

#include "xmlParser.h"
#include "KVCache.h"


#define MAXKEYSIZE 260
#define MAXVALUESIZE 262150

void serveRequest(char buffer[], int *rc, int *fd)
{
    char key[MAXKEYSIZE]={0}, value[MAXVALUESIZE]={0}, msg[50]={0}, msgType[10]={0};
  	
    ParseXML(msg, msgType, key, value, buffer);

  	char *response;

  	// printf("key - %ld\n",strlen(key));
  	// printf("msgType - %s\n",msgType);
  	// printf("key - %s\n",key);
  	// printf("value - %s\n\n",value);

  	int flag = -1;

    if(strlen(key) > 256)
    {
      strcpy(msgType, "resp");
      strcpy(msg, "Oversized key");
      response = GenerateXML("", "", msg, msgType);
      // puts(response);
    }
    else if(strlen(value) > 256*1024)
    {
      strcpy(msgType, "resp");
      strcpy(msg, "Oversized value");
      response = GenerateXML("", "", msg, msgType);
      // puts(response);
    }
    else
    {

      if(!strcmp(msgType, "putreq"))
      {
        strcpy(msgType, "resp");
        putData(key, value);
        strcpy(msg, "Success");
        response = GenerateXML("", "", msg, msgType);
        // puts(response);
      }
      else if(!strcmp(msgType, "getreq"))
      {
        strcpy(msgType, "resp");
        char *temp = getData(key);

        if(temp)
        {
          strcpy(value, temp);
          strcpy(msg, "");
        }
        else
        {
          strcpy(value, "");
          strcpy(key, "");
          strcpy(msg, "Does not exist");
        }
        
        response = GenerateXML(key, value, msg, msgType);
      }
      else if(!strcmp(msgType, "delreq"))
      {
        strcpy(msgType, "resp");
        if(delData(key) == 1)
        {
          strcpy(msg, "Success");
        }
        else
        {
          strcpy(msg, "Does not exist");
        }
        response = GenerateXML("", "", msg, msgType);
      }

    }
  	
  	
  	response[strlen(response)] = '\0';

    printf("ThreadID : %ld\n\n", pthread_self());

    *rc = send(*fd, response, strlen(response), 0);
}