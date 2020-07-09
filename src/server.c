#include "server.h"

//int exec_cmd(int client_socket,char *cmd);

int server(char *addr, int port) {

	printf("SERVER\n");
	char message[MAX] = "Bienvenue sur le serveur";
	char * pointer_message = message;
	
	//Reperatoire par defaut des documents
	char chemin[MAX] = "../Documents"; 
	 
	//liste contenant le nom des fichiers dans le repertoire
	char  *nom_fichier[MAX]= {"test", "test"};
	char  **pointer = nom_fichier;
	
	char temp[MAX];
	printf("le chemin vers le dossier documents (0 par defaut): ");
	scanf("%s", temp);
	if (strcmp(temp,"0") != 0){
		bzero(chemin,MAX);
		strcpy(chemin,temp);
		bzero(temp,MAX);
	}
	get_files(chemin,&pointer);

	//initialisation du socket serveur 
	int serv_socket;
	serv_socket = socket(AF_INET,SOCK_STREAM,0);
	
	//declaration des variables pour stocker les adresses
	struct sockaddr_in server_address,cli_addr;
	socklen_t addr_size = sizeof(struct sockaddr_in);

	//Remplissage des informations de l'adresse serveur
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port); //port par defaut :8080
	inet_pton(AF_INET,addr,&server_address.sin_addr.s_addr);//adresse par defaut : localhost

	//liaison du socket serveur sur le port et l'addresse
	int status = bind(serv_socket, (struct sockaddr*) &server_address, sizeof(server_address));

	
	if (status != 0){
		printf("Erreur de l'initialisation du serveur \n");
		return -1;
	}
	
	
	while(1){ 
		
		printf("Server operationel sur %s\n",inet_ntoa((struct in_addr) server_address.sin_addr));
		printf("Port %d\n", port);
		// mise en ecoute du serveur
		listen(serv_socket, 5);
		int client_socket = accept(serv_socket,(struct sockaddr*) &cli_addr, &addr_size);
		
		printf("%s est connecte \n",inet_ntoa((struct in_addr) cli_addr.sin_addr));
		printf("Socket: %d \n", client_socket);
		
		
		//envoie du message de bienvenue 
		write(client_socket, message, sizeof(message));
		printf("Message de bienvenue envoye\n");

		
		//debut du chat client-server
		chat_server(client_socket,&pointer, chemin);
		printf("Attendre un autre client ?(y/n): ");
		char continuer;
		getchar();
		scanf("%c",&continuer);
		
		if(continuer == 'n'){
		
			printf("Deconnexion...\n");
			
			break;
		}
		
	}

	//fermeture du socket serveur
	int rc = close(serv_socket);

	return 0;

	
}



int chat_server(int sockfd, char ***nom_fichier, char *chemin) 
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
		bzero(*nom_fichier,MAX);
		get_files(chemin,nom_fichier);

		bzero(buff, sizeof(buff)); 
		if(read(sockfd, buff, sizeof(buff))==0)
			return 0;
			

		if (strcmp(buff,"ls") == 0){
		//Envoyer la liste des documents
		bzero(buff, sizeof(buff));
		printf("Envoi de la liste...\n");
			for (int i=0;(*nom_fichier)[i];i++){
				
				if((strcmp((*nom_fichier)[i],".")==0) || (strcmp((*nom_fichier)[i],"..")==0))
				continue; //Ne pas envoyer . et ..
				bzero(buff, sizeof(buff)); 
				strcpy(buff,(*nom_fichier)[i]);
				printf("%s \n",buff);
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
			if(read(sockfd, buff, sizeof(buff)) == 0){
				return 0;
			}
			printf("Suppression de %s \n",buff);
			char chemin_fichier[1024];
			strcat(chemin,"/");
			strcpy(chemin_fichier,chemin);
			strcat(chemin_fichier,buff);
			int status = remove(chemin_fichier);
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
			bzero(buff, sizeof(buff));
			//Obtenir le nom du document a telecharger
			printf("Telechargement\n");
			strcpy(buff,"entrez le nom du document:");
			write(sockfd,buff,sizeof(buff));
			end_message(sockfd);
			
			bzero(buff, sizeof(buff));
			if(read(sockfd, buff, sizeof(buff))==0){
				printf("Erreur %s\n",buff);				
				return 0;
			}

			
			
			//envoie du fichier
			Envoyer_fichier(sockfd,buff,chemin);
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

int Envoyer_fichier(int connfd, char *fname,char *chemin)
{
      	char chemin_fichier[1024];
	strcat(chemin,"/");
	strcpy(chemin_fichier,chemin);
	strcat(chemin_fichier,fname);
	printf("Chemin du fichier %s\n",chemin_fichier);
	FILE *fp = fopen(chemin_fichier,"r");
        if(fp==NULL)
        {
		printf("Erreur de l'ouverture du fichier \n");
		write(connfd,"Erreur de l'ouverture du fichier",MAX);
		return 1;   
        } 

	//Envoi signal de telechargement
	
	write(connfd,"TeLeCharGemEnt",MAX);

	//Envoi nom du fichier
	printf("Le nom du fichier: %s\n",fname);
	write(connfd, fname,MAX);

  
        while(1)
        {
       
            unsigned char buff[1024]={0};
            int nread = fread(buff,1,1024,fp);
  
            if(nread > 0)
            {
           
                write(connfd, buff, nread);
            }
            if (nread < 1024)
            {
                if (feof(fp))
		{
                    printf("End of file\n");
		    printf("File transfer completed for id: %d\n",connfd);
			
		}
                if (ferror(fp))
                    printf("Error reading\n");
		
		
                break;
		
            }
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
