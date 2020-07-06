#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <dirent.h>
#ifndef MAX
#define MAX 256
#endif
void show_files(char* chemin);
void get_files(char* chemin, char ***nom_fichier);
int chat_server(int sockfd, char ***nom_fichier, char *chemin);
void end_message(int client_socket);
void send_help(int client_socket);
int Envoyer_fichier(int connfd, char *fname,char *chemin);
int server(char *addr,int port);
