#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#ifndef MAX
#define MAX 256
#endif
int client(char *addr, int port);
void ecouter_serv(int cli_socket);
void chat_client(int sockfd);
int telecharger(int sockfd, char* fname);

