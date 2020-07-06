# client-server_JOHANESA_ASSUINE


Ce programme client-server est destiné à gérer et télécharger des documents depuis un ordinateur serveur. 


# Fonctionnalités

  - ls : lister les documents du serveur
  - rm : supprimer un document dans le serveur
  - get : prendre un document depuis le serveur
  - exit : quitter le serveur 


### Tech

Le programme client-server utilise les librairies suivants :

* <stdio.h>
* <stdlib.h>
* <sys/types.h>
* <sys/socket.h>
* <netinet/in.h>
* <unistd.h>
* <string.h>
* <arpa/inet.h>
* <dirent.h>


#### Compilation
Afin de compiler le programme :

$ cd bin
$ cmake ..
$ make

ou

$ gcc -I includes -o bin/main src/main.c src/client.c src/server.c
```
### Lancement
Pour lancer le programme il faut aller dans /bin puis lancer le main.
Les arguments positionnels sont comme suit :
   
```
$./main client adresse port
ou
$./main server adresse port
```
Mais il peut être lancé sans argument positionel 

```


### Todos

 - Correction : Plantage recurrent sur l'ecriture du document sur le client 


