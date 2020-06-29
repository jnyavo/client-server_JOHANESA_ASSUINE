#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>


int main() {


	//initialisation du socket serveur 
	int socket;
	socket = socket(AF_INET,SOCK_STREAM,0);
	
	//declaration des variables pour les adresses
	struct sockaddr_in server_address,cli_addr;

	//Remplissage des informations de l'adresse serveur
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(8080); //port 8080
	server_address.sin_addr.s_addr = INADDR_ANY;//adresse par defaut

	//liaison du socket serveur sur le port et l'addresse
	int status = bind(socket, (struct sockaddr*) &server_address, sizeof(server_address));

	
	if (status != 0){
		printf("Erreur de l'initialisation du serveur");
		return -1;
	}


	// mise en ecoute du serveur
	listen(socket, 5);
	int client_socket = accept(socket,(struct sockaddr*) &cli_addr, NULL);
	char message[256] = "Bienvenue sur le serveur";

	//envoie du message de bienvenue 
	send(client_socket, message, sizeof(message),0);


	//fermeture du socket serveur
	close(socket);

	return 0;
	
}
