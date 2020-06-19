#include <stdio.h> //printf, scanf
#include <sys/types.h> //defines a number of data types used in system call
#include <sys/socket.h> //defines a number of structures of socket (sockaddr)
#include <netinet/in.h> //defines constants and structures (sockaddr_in)
#include <stdlib.h> //define var_types, macros, funcs etc. (atio())
#include <netdb.h> //structure hostent
#include <unistd.h> //read, write, close
#include <string.h>

void error(const char *mssg)
{
	perror(mssg);
	exit(1);
}

int main(int argc, char* argv[])
{
	//if user provided 3 agrs
	if(argc < 3)
		error("Server Ip and Port number must be included.");
		
	int client_socket, PORT, n;
	struct sockaddr_in server_addr;
	struct hostent *server;
	char buffer[256]; // for message 
	
	PORT = atoi(argv[2]);
	
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket < 0) 
        	error("ERROR opening socket");
	
	server = gethostbyname(argv[1]);
	if (server == NULL)
		error("No such host.");
		
	bzero((char *) &server_addr, sizeof(server_addr));
	//specifying server
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	bcopy( (char*)server->h_addr, (char*)&server_addr.sin_addr.s_addr, server->h_length );
	
	//connect to the server
	n = connect(client_socket, (struct sockaddr*) &server_addr, sizeof(server_addr));
	if (n < 0) 
        	error("ERROR connecting");
	printf("[CONNECTED]\n");
	
	while(1)
	{
		/*Client will send message first*/
		
		//send
		bzero(buffer, 256); //clearing prev mssg;
		fgets(buffer, 256, stdin);//getting mssg from cosole
		n = write(client_socket, buffer, strlen(buffer));//sending data
		if (n < 0) 
             		error("ERROR writing to socket");
             	printf("[SENT]\n");
		
		//recving
		bzero(buffer, 256); //clearing prev mssg;
		n = read(client_socket, buffer, 256);//rcving data
		if (n < 0) 
         		error("ERROR reading from socket");
         	printf("Server : %s\n",buffer);
         	
         	//code for breaking loop
         	int i = strncmp("quit" , buffer , 4);
        	if(i == 0)
               		break;
	}
	
	close(client_socket);
	return 0;
}
