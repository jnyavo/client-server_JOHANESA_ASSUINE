#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define MAX 256


void ecouter_serv(int cli_socket);
void chat_client(int sockfd);

  

int main() {

	printf("CLIENT \n");

	//Creation du socket
	int cli_socket;
	cli_socket = socket(AF_INET,SOCK_STREAM,0);
	
	//Les informations sur le serveur
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(8080); //port 8080
	server_address.sin_addr.s_addr = INADDR_ANY; // adresse par defaut

	//Connection au serveur
	printf("Connexion au server... \n");
	if ( connect(cli_socket,(struct sockaddr*) &server_address,sizeof(server_address)) != 0 ){
		printf("Erreur de connexion \n");
		exit(-1);
	
	}
	printf("Connexion etablie. \n");
	
	//Reception de la reponse du serveur 
	char reponse_serv[256];	
	recv(cli_socket, reponse_serv, sizeof(reponse_serv), 0);
	
	//Affichage de la reponse
	printf("le message est : %s \n",reponse_serv);
	
	//debut du chat client-server
	chat_client(cli_socket);
	
	//fermeture du socket
	close(cli_socket);


	return 0;
}

void chat_client(int sockfd) 
{ 
    
	char message[MAX];
	char input[MAX];
	

	
	while (1) { 

		
		bzero(message, MAX); 
		
		//ecouter le serveur jusqu'a ce qu'il a fini
		ecouter_serv(sockfd);
		
		//repondre
		printf(">");
		scanf("%s",input);
		strcpy(message,input);

		
		write(sockfd,message,sizeof(message));
		if(strcmp(message,"exit") == 0)
			break;
		
	} 
    
} 

void ecouter_serv(int cli_socket){
//Ecouter le serveur
	char message[MAX] = "";

	while (1){
		bzero(message, MAX);
		read(cli_socket, message, sizeof(message));
		if (strcmp(message,"fin") == 0)
			break; //le serveur a communique la fin
		printf("\t%s\n", message); 
		
	}
	

}
