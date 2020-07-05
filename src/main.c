#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

int main() {
	FILE *p;
	char buff[256];
	p = popen("rm rien","r");
	
	for(int i=1;i<5;i++) {
	bzero(buff,256);
	fgets(buff,256,p);

	printf("a : %s",buff);
	}
	pclose(p);
	printf("Fin \n");
	return 0;
}
