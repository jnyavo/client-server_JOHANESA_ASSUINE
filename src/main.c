
#include "server.h"
#include "client.h"



int main(int argc, char *argv[]) {
	int choix;
	char addr[50];
	int port = 0;
	char str_port[50];
	printf("\n");
	if (argc < 2){
		printf("Aucun argument positionel : main <client/server> <addr> <port>\n");
		printf("Voulez vous etre client ou serveur ?\n1: client\n2: serveur\n>");
		scanf("%d",&choix);
		
		switch(choix){
			case 1:
				
				printf("Adresse du serveur (0 pour localhost): ");
				scanf("%s",addr);
				printf("Port du serveur (0 pour 8080): ");
				scanf("%s",str_port);
				port = atoi(str_port);
				
				if(port==0)
					port = 8080;
				
				
				if(strcmp(addr,"0") == 0)
					strcpy(addr,"127.0.0.1");
				
				client(addr,port);
			break;
			
			case 2:
				printf("Adresse et port par defaut\n Aucun argument positionel : main <client/server> <addr> <port>\n");
				server("127.0.0.1",8080);
			break;
			default:
				printf("Choix incorrect (1 ou 2)\n");
			break;
			
			
		}
	}
	else if (argc < 3){
	
		if (strcmp(argv[1],"client") == 0){
			printf("Adresse du serveur (0 pour localhost): ");
			scanf("%s",addr);
			printf("Port du serveur (0 pour 8080): ");
			scanf("%s",str_port);
			port = atoi(str_port);
			if(strcmp(addr,"0") == 0)
					strcpy(addr,"127.0.0.1");
			if(port==0)
				port = 8080;
			

			client(addr,port);

		} else if(strcmp(argv[1],"server") == 0){
			printf("Adresse et port par defaut\n Aucun argument positionel : main <client/server> <addr> <port>\n");
			server("127.0.0.1",8080);
			
		} else {
			printf("%s inconnu (client/server)\n", argv[1]);
		}
	
	}
	else if (argc < 4) {
		printf("Port par defaut : 8080\n");	
		strcpy(addr,argv[2]);
		port = 8080;
		if (strcmp(argv[1],"client") == 0){
			
			
			
			client(addr,port);

		} else if(strcmp(argv[1],"server") == 0){
			
			server(addr,port);
			
		} else {
			printf("%s inconnu (client/server)\n", argv[1]);
		}
		
	
	}
	else {
		strcpy(addr,argv[2]);
		port = atoi(argv[3]);

		if (strcmp(argv[1],"client") == 0){
			
			
			
			client(addr,port);

		} else if(strcmp(argv[1],"server") == 0){
			
			server(addr,port);
			
		} else {
			printf("%s inconnu (client/server)\n", argv[0]);
		}
		

	}
	
	
	
	return 0;
}
