#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>



#include "threadpool.h"
#include "serveRequest.h"

#define MAXKEYSIZE 260
#define MAXVALUESIZE 262150
#define MAXBUFSIZE 264000


/* Coment this macro to stop generating CacheXML file */
#define PRINTCACHE 1



#define TRUE             1
#define FALSE            0

size_t num_threads = 4;


void main (int argc, char *argv[])
{
  int    len, rc, on = 1;
  int    listen_sd = -1, new_sd = -1;
  int    desc_ready, end_server = FALSE, compress_array = FALSE;
  int    close_conn;
  char   buffer[MAXBUFSIZE];
  struct sockaddr_in   addr;
  int    timeout;
  struct pollfd fds[200];
  int    nfds = 2, current_size = 0, i, j;
  int port = 8080;

  if(argc >= 2)
    port = atoi(argv[1]);
  if(argc >= 3)
    sets = atoi(argv[2]);
  if(argc >= 4)
    nway = atoi(argv[3]);
  if(argc >= 5)
	num_threads = atoi(argv[4]);

  
  cacheInit();

  tpool_t *tm;
  tm   = tpool_create(num_threads);

  listen_sd = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_sd < 0)
  {
    perror("Network Error: Could not create socket");
    exit(-1);
  }

  rc = setsockopt(listen_sd, SOL_SOCKET,  SO_REUSEADDR,
                  (char *)&on, sizeof(on));
  if (rc < 0)
  {
    perror("setsockopt() failed");
    close(listen_sd);
    exit(-1);
  }


  rc = ioctl(listen_sd, FIONBIO, (char *)&on);
  if (rc < 0)
  {
    perror("ioctl() failed");
    close(listen_sd);
    exit(-1);
  }


  memset(&addr, 0, sizeof(addr));
  addr.sin_family      = AF_INET;
  //memcpy(&addr.sin_addr, &INADDR_ANY, sizeof(INADDR_ANY));
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port        = htons(port);
  rc = bind(listen_sd,
            (struct sockaddr *)&addr, sizeof(addr));
  if (rc < 0)
  {
    perror("bind() failed");
    close(listen_sd);
    exit(-1);
  }


  rc = listen(listen_sd, 32);
  if (rc < 0)
  {
    perror("listen() failed");
    close(listen_sd);
    exit(-1);
  }

  memset(fds, 0 , sizeof(fds));


  fds[0].fd = listen_sd;
  fds[0].events = POLLIN;
  timeout = (50 * 100);

  do
  {
    printf("---------------------------------------\n");
    rc = poll(fds, nfds, timeout);
    if (rc < 0)
    {
      perror("  poll() failed");
      break;
    }

    if (rc == 0)
    {
      printf("  poll() timed out.  End program.\n");
      break;
    }

    current_size = nfds;
    for (i = 0; i < current_size; i++)
    {
      if(fds[i].revents == 0)
        continue;

      if(fds[i].revents != POLLIN)
      {
        printf("  Error! revents = %d\n", fds[i].revents);
        // end_server = TRUE;
        // break;

      }
      if (fds[i].fd == listen_sd)
      {
        printf("  Listening socket is readable\n");
        do
        {
          new_sd = accept(listen_sd, NULL, NULL);
          if (new_sd < 0)
          {
            if (errno != EWOULDBLOCK)
            {
              perror("  accept() failed");
              // end_server = TRUE;
            }
            // break;
          }
          printf("  New incoming connection - %d\n", new_sd);
          fds[nfds].fd = new_sd;
          fds[nfds].events = POLLIN;
          nfds++;
        } while (new_sd != -1);
      }
      else
      {
        printf("  Descriptor %d is readable\n", fds[i].fd);
        close_conn = FALSE;
        
        // do
        // {
          memset(buffer, 0, sizeof(buffer));
          rc = recv(fds[i].fd, buffer, sizeof(buffer), 0);
          if (rc < 0)
          {
            if (errno != EWOULDBLOCK)
            {
              perror("  recv() failed");
              close_conn = TRUE;
            }
            //break;
          }
          else if (rc == 0)
          {
            printf("  Connection closed\n");
            close_conn = TRUE;
            // break;
          }
          else
          {
            len = rc;
            printf("  %d bytes received\n", len);
          
            tpool_add_work(tm, serveRequest, buffer, &rc, &fds[i].fd);
            usleep(1000);
            
            if (rc < 0)
            {
              perror("  send() failed");
              close_conn = TRUE;
            
            }
          }

      
          
          

        // } while(TRUE);

        if (close_conn)
        {
          close(fds[i].fd);
          fds[i].fd = -1;
          compress_array = TRUE;
        }


      }  /* End of existing connection is readable             */
    } /* End of loop through pollable descriptors              */
    if (compress_array)
    {
      compress_array = FALSE;
      for (i = 0; i < nfds; i++)
      {
        if (fds[i].fd == -1)
        {
          for(j = i; j < nfds; j++)
          {
            fds[j].fd = fds[j+1].fd;
          }
          i--;
          nfds--;
        }
      }
    }

  } while (end_server == FALSE); /* End of serving running.    */
  tpool_wait(tm);
  tpool_destroy(tm);

  for (i = 0; i < nfds; i++)
  {
    if(fds[i].fd >= 0)
      close(fds[i].fd);
  }

  #ifdef PRINTCACHE
  toXML("ServerData/CacheXML.xml");
  #endif
}

