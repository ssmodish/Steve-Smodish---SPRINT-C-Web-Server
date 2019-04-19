#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "lib.h"

#define BUFSIZE 4096 // max number of bytes we can get at once

/**
 * Struct to hold all three pieces of a URL
 */
typedef struct urlinfo_t {
  char *hostname;
  char *port;
  char *path;
} urlinfo_t;

/**
 * Tokenize the given URL into hostname, path, and port.
 *
 * url: The input URL to parse.
 *
 * Store hostname, path, and port in a urlinfo_t struct and return the struct.
*/
urlinfo_t *parse_url(char *url)
{
  // copy the input URL so as not to mutate the original
  char *hostname = strdup(url);
  char *port;
  char *path;

  urlinfo_t *urlinfo = malloc(sizeof(urlinfo_t));

  /*
    We can parse the input URL by doing the following:

    1. Use strchr to find the first backslash in the URL (this is assuming there is no http:// or https:// in the URL).
    2. Set the path pointer to 1 character after the spot returned by strchr.
    3. Overwrite the backslash with a '\0' so that we are no longer considering anything after the backslash.
    4. Use strchr to find the first colon in the URL.
    5. Set the port pointer to 1 character after the spot returned by strchr.
    6. Overwrite the colon with a '\0' so that we are just left with the hostname.
  */


  ///////////////////
  // IMPLEMENT ME! //
  ///////////////////
  printf("parse_url 1.0: hostname - %s\n", hostname);
  char *divider = strchr(hostname, '/');
  if(divider){
    path = (divider + 1);
    printf("parse_url 1.1: path - %s\n", path);
    hostname = strtok(hostname, "/");
  }
  printf("parse_url 1.2: hostname - %s\n", hostname);
  divider = strchr(hostname, ':');
  if(divider){
    port = (divider + 1);
    printf("parse_url 1.3: port - %s\n", port);
    hostname = strtok(hostname, ":");
  }
  printf("parse_url 1.4: hostname - %s\n", hostname);

  urlinfo->hostname = hostname;
  urlinfo->port = port;
  urlinfo->path = path;

  return urlinfo;
}

/**
 * Constructs and sends an HTTP request
 *
 * fd:       The file descriptor of the connection.
 * hostname: The hostname string.
 * port:     The port string.
 * path:     The path string.
 *
 * Return the value from the send() function.
*/
int send_request(int fd, char *hostname, char *port, char *path)
{
  const int max_request_size = 16384;
  char request[max_request_size];
  int rv;

  ///////////////////
  // IMPLEMENT ME! //
  ///////////////////
  printf("send_request 1.0\n");

  sprintf(request,  "GET /%s HTTP/1.1\n"
                    "Host: %s:%s\n"
                    "Connection: close", path, hostname, port);

  printf("send_request 1.1 REQUEST\n%s\n", request);

  fd = get_socket(hostname, port);
  printf("send_request 1.2 SOCKET: %d\n", fd);

  return send(fd, request, strlen(request), 0);

}

int main(int argc, char *argv[])
{  
  int sockfd, numbytes;  
  char buf[BUFSIZE];

  if (argc != 2) {
    fprintf(stderr,"usage: client HOSTNAME:PORT/PATH\n");
    exit(1);
  }

  /*
    1. Parse the input URL
    2. Initialize a socket by calling the `get_socket` function from lib.c
    3. Call `send_request` to construct the request and send it
    4. Call `recv` in a loop until there is no more data to receive from the server. Print the received response to stdout.
    5. Clean up any allocated memory and open file descriptors.
  */

  ///////////////////
  // IMPLEMENT ME! //
  ///////////////////
  printf("main 1.0\n");
  urlinfo_t *urlinfo = parse_url(argv[1]);
  printf("main 1.1\n");
  printf("urlinfo->hostname: %s\n", urlinfo->hostname);
  printf("urlinfo->port: %s\n", urlinfo->port);
  sockfd = get_socket(urlinfo->hostname, urlinfo->port);
  printf("main 1.2\n");
  send_request(sockfd, urlinfo->hostname, urlinfo->port, urlinfo->path);
  printf("main 1.3\n");
  recv(sockfd, buf, BUFSIZE, 0);
  printf("main 1.4\n");
  while(buf > 0){
    printf("main 2.0\n");
    printf("%s", buf);
    printf("main 2.1\n");
    recv(sockfd, buf, BUFSIZE, 0);
  }
  printf("main 1.5\n");

  return 0;
}
