#include "client.h"
#include "server.h"

int client(char *addr, int port) {

	printf("CLIENT \n");

	//Creation du socket
	int cli_socket;
	cli_socket = socket(AF_INET,SOCK_STREAM,0);
	
	//Les informations sur le serveur
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port); //port par defaut 8080
	inet_pton(AF_INET,addr,&server_address.sin_addr.s_addr); // adresse par defaut

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
		
		if (strcmp(message,"TeLeCharGemEnt") == 0)
		{
			//Le serveur a communique un telechargement
			printf("Fichier ouvert, telechargement...\n");
			telecharger(cli_socket,message);	
		
		}
		if (strcmp(message,"fin") == 0)
			break; //le serveur a communique la fin d'un message

		
		printf("\t%s\n", message); 

		
	}
	

}

int telecharger(int sockfd, char* fname){
	 //Creer un fichier ou l'on va stocker les donnees telecharges
	int taille = 0;
	char recvBuff[1024];
	memset(recvBuff, '0', sizeof(recvBuff));	
	
	FILE *fp;
	read(sockfd, fname, MAX);
	strcat(fname,".copy");
	printf("File Name: %s\n",fname);
	printf("Reception du fichier...\n");
	fp = fopen(fname, "ab"); 
	if(NULL == fp)
	{
		printf("Erreur d'ecriture");
		return 1;
	}
	while (1){
	taille = read(sockfd, recvBuff, 1024);
	printf("%s",recvBuff);
	if (strcmp(recvBuff,"fin") == 0)
		break; //le serveur a communique la fin d'un message
		
	fwrite(recvBuff, 1,1024,fp);
	
	}
	if(taille < 0)
	{
		printf("\n Erreur de lecture \n");
	}
		else {	
		printf("\nTelechargement reussi.\n");
		}
		
	return 0;


}
