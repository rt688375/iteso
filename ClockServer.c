/**
*  Oscar Alfredo Reyes Espinosa.
**/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

int main(int args, char *argv[]) {

	u_int port;
	int server;
	int client;
	int localerror;
	struct sockaddr_in myAddr;
	struct sockaddr_in cliente_addr;
	socklen_t clienteLen;	
	int status;
	time_t tiempo = time(0);
	struct tm *tlocal = localtime(&tiempo);
	char output [128];
	strftime (output, 128, "%d/%m/%y %H:%M:%S\n", tlocal);
	char hora[] = "La hora actual es: ";
	char saludo[] = "-----------Conexion Establecida-----------\n";
	
	//Validamos los Arguemntos
	if(args < 2) {
		fprintf(stderr,"Error: Missing Arguments\n");
		fprintf(stderr,"\tUSE: %s [PORT]\n",argv[0]);
		return 1;
	}

	port = atoi(argv[1]);
	if(port < 1 || port > 65535) {
		fprintf(stderr,"Port %i out of range (1-65535)\n",port);
		return 1;
	}

//Iniciamos la apertura del Socket
	server = socket(PF_INET,SOCK_STREAM,0);
	if(server == -1) {
		localerror = errno;
		fprintf(stderr, "Error: %s\n",strerror(localerror));
		return 1;
	}

//Nos adjudicamos el Puerto.
	bzero(&myAddr,sizeof(myAddr));
	myAddr.sin_family = AF_INET;
	myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myAddr.sin_port = htons(port);

	status = bind(server, (struct sockaddr *)&myAddr, sizeof(myAddr));
	if(status != 0) {
		localerror = errno;
		fprintf(stderr,"Can't Bind Port: %s\n",strerror(localerror));
		close(server);
		return 1;
	}

//Nos ponemos el modo de Escucha
	status = listen(server,5);
	if(status == -1) {
		localerror = errno;
		fprintf(stderr,"Can't listen on socket(%s)\n",strerror(localerror));
		close(server);
		return 1;
	}	

//Esperamos una Conexión
	while(1) {
		bzero(&cliente_addr,sizeof(cliente_addr));
		client = accept(server,(struct sockaddr *)&cliente_addr,&clienteLen);
		printf("-----------Conexion Establecida...-----------\n\n");
		status = write(client, saludo, strlen(saludo));
		if(client == -1) {
			localerror = errno;
			fprintf(stderr,"Error acepting conection %s\n",strerror(localerror));
			close(server);
			return 1;
		}

//Inicia el protocolo para mandar la hora...
			printf("Enviando hora...\n\n");		
			status = write(client,hora, strlen(hora));
			status = write(client, output, strlen(output));
			printf("Hora enviada...\n\n");

		close(client);
	}

	return 0;
}
