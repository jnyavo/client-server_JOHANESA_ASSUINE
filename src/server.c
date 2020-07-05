#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <dirent.h>

#define MAX 256

void show_files(char* chemin);
void get_files(char* chemin, char ***nom_fichier);
void chat_server(int sockfd, char ***nom_fichier);
void end_message(int client_socket);
void send_help(int client_socket);


//int exec_cmd(int client_socket,char *cmd);

void func(int sockfd) 
{ 
	char buff[MAX]; 
	int n; 
	for (;;) { 
		bzero(buff, sizeof(buff)); 
		printf("Enter the string : "); 
		n = 0; 
		while ((buff[n++] = getchar()) != '\n') 
			; 
		write(sockfd, buff, sizeof(buff)); 
		bzero(buff, sizeof(buff)); 
		read(sockfd, buff, sizeof(buff)); 
		printf("From Server : %s", buff); 
		if ((strncmp(buff, "exit", 4)) == 0) { 
			printf("Client Exit...\n"); 
			break; 
		} 
	} 
} 

int main() {

	
	char message[256] = "Bienvenue sur le serveur";
	char * pointer_message = message; 
	char  *nom_fichier[256]= {"test", "test"};
	char  **pointer = nom_fichier;
	
	
	get_files(".",&pointer);

	//initialisation du socket serveur 
	int serv_socket;
	serv_socket = socket(AF_INET,SOCK_STREAM,0);
	
	//declaration des variables pour stocker les adresses
	struct sockaddr_in server_address,cli_addr;
	socklen_t addr_size = sizeof(struct sockaddr_in);

	//Remplissage des informations de l'adresse serveur
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(8080); //port 8080
	server_address.sin_addr.s_addr = INADDR_ANY;//adresse par defaut

	//liaison du socket serveur sur le port et l'addresse
	int status = bind(serv_socket, (struct sockaddr*) &server_address, sizeof(server_address));

	
	if (status != 0){
		printf("Erreur de l'initialisation du serveur \n");
		return -1;
	}

	while(1){ 
		// mise en ecoute du serveur
		listen(serv_socket, 5);
		int client_socket = accept(serv_socket,(struct sockaddr*) &cli_addr, &addr_size);
		
		printf("%s est connecte \n",inet_ntoa((struct in_addr) cli_addr.sin_addr));
		printf("Socket: %d \n", client_socket);
		
		
		//envoie du message de bienvenue 
		write(client_socket, message, sizeof(message));
		printf("Message de bienvenue envoye\n");

		
		//debut du chat client-server
		chat_server(client_socket,&pointer);
		printf("Attendre un autre client ?(y/n): ");
		char continuer;
		scanf("%c",&continuer);
		getchar();
		if(strncmp(&continuer,"y",1))
			break;
	}

	//fermeture du socket serveur
	int rc = close(serv_socket);

	return 0;

	
}



void chat_server(int sockfd, char ***nom_fichier) 
{ 
	//string pour les messages
	char buff[MAX];
	char req[MAX];
	
	// envoi des requetes/commandes disponibles au client
	send_help(sockfd);
	end_message(sockfd);
	
	printf("Traitement en cours...\n");
	//Traitement des requetes
	while (1) { 
		//actualisation de la liste de documents
		get_files(".",nom_fichier);

		bzero(buff, sizeof(buff)); 
		read(sockfd, buff, sizeof(buff));

		if (strcmp(buff,"ls") == 0){
		//Envoyer la liste des documents
		bzero(buff, sizeof(buff));
		printf("Envoi de la liste...\n");
			for (int i=0;(*nom_fichier)[i];i++){
				//Ne pas envoyer . et ..
				if((strcmp((*nom_fichier)[i],".")==0) || (strcmp((*nom_fichier)[i],"..")==0))
				continue;
				bzero(buff, sizeof(buff)); 
				strcpy(buff,(*nom_fichier)[i]);
				printf("le fichier est: %s \n",buff);
				write(sockfd,buff, sizeof(buff));
			}
		//Fin de message
		end_message(sockfd);
		}
		else if (strcmp(buff,"rm") == 0){
		//Supprimer le document
			bzero(buff, sizeof(buff));
			//Obtenir le nom du document a supprimer)
			printf("Suppression\n");
			strcpy(buff,"entrez le nom du document:");
			write(sockfd,buff,sizeof(buff));
			
			end_message(sockfd);
			
			
			bzero(buff, sizeof(buff));
			read(sockfd, buff, sizeof(buff));
			printf("Suppression de %s \n",buff);
			int status = remove(buff);
			if(status == 0){
				printf("Suppression effectuee \n");
				strcpy(buff,"Suppression effectuee");
				write(sockfd,buff,sizeof(buff));		
			}
			else
			{
				printf("Echec de suppresion\n");
				strcpy(buff,"Echec de suppresion");
				write(sockfd,buff,sizeof(buff));
			}
			end_message(sockfd);
		}
		else if (strcmp(buff,"get") == 0){
		//Envoyer le document
			
			strcpy(buff,"test");
			write(sockfd,buff,sizeof(buff));
			end_message(sockfd);
			
		}
		
		else if ((strcmp(buff, "exit")) == 0) { 
			close(sockfd);
			printf("Client Exit...\n"); 
			break; 
		} 
		else
		{
			printf("Commande inconnue\n");
			send_help(sockfd);
			end_message(sockfd);
		}
		
		
	} 
} 

void end_message(int client_socket){
	
	//Informer le client qu'il peut arreter d'ecouter
	char buff[] = "fin";
	write(client_socket,buff,sizeof(buff));
	printf("fin de message\n");
	 
}

void send_help(int client_socket){
	//envoi des requetes disponibles
	printf("envoi des requetes disponible...\n");
	char buff[MAX];
	strcpy(buff,"COMMANDES\nls : liste des documents\nrm : supprimer un document\nget: Obtenir un document\nexit : fin du programme");
	write(client_socket, buff, sizeof(buff)); 
	printf("requete envoye\n");
}



void get_files(char* chemin,char ***nom_fichier){
//prendre le nom des fichiers dans le repertoire
	DIR *d;
	struct dirent *dir;
	d = opendir(chemin);
	if (d) {
	for(int i=0; (dir = readdir(d)) != NULL; i++) {
		

		(*nom_fichier)[i] = dir->d_name; 

	}

	closedir(d);
}
}

/*
int exec_cmd(int client_socket,char *cmd){
//execute une commande et envoie le resultat au client
	FILE *fp;
	char buff[MAX];
	fp = popen(cmd,"r");
	if((fp == NULL)){
		return -1;	
	}
	
	while (fgets(buff,MAX,fp) != NULL){
		write(client_socket,buff,sizeof(buff));
		bzero(buff, sizeof(buff));	
				
	}
	
	if (pclose(fp)){
		strcpy(buff,"le fichier n'existe pas");
		write(client_socket,buff,sizeof(buff));
		bzero(buff, sizeof(buff));
	}
	return 0;
	

}
*/
