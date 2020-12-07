#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include  <unistd.h>
#include <dlfcn.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUFFER_LEN 1024

int filterhost(char* host){
  if(strstr(host, "akamai")) return 1;
  if(strstr(host, "googleusercontent")) return 1;

  return 0;
}

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen){

  int (*orig_conn) (int, const struct sockaddr*, socklen_t) = NULL;
  orig_conn = dlsym(RTLD_NEXT, "connect");
  printf("hijacking \n");
  char *fam = "OTHER";

  struct sockaddr_in *myaddr = (struct sockaddr_in*)addr;
  struct sockaddr_in6 *myaddr6 = (struct sockaddr_in6*)addr;

  if(addr ->sa_family == AF_INET)
  fam = "AF_INET";
  if(addr ->sa_family == AF_INET6)
  fam = "AF_INET6";

  char address_str[BUFFER_LEN] = {0};

  if(addr->sa_family == AF_INET){
    inet_ntop(addr->sa_family, &(myaddr->sin_addr.s_addr), address_str, BUFFER_LEN);
  }
  else if(addr->sa_family == AF_INET6){
    inet_ntop(addr->sa_family, &(myaddr6->sin6_addr.s6_addr), address_str, BUFFER_LEN);
  }
   char host[BUFFER_LEN] ={0};
   char server[BUFFER_LEN] ={0};

   getnameinfo(addr, addrlen, host, BUFFER_LEN, server, BUFFER_LEN, 0);



  // printf("%s (%s) %s\n", host, fam, address_str);
  // printf(host);

  if(filterhost(host)){
    return -1;
  }

  return orig_conn(sockfd, addr, addrlen);
  // errno = ECONNREFUSED;
  // return -1;
}
