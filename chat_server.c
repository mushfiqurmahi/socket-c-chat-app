#include <stdio.h> //printf, scanf
#include <sys/types.h> //defines a number of data types used in system call
#include <sys/socket.h> //defines a number of structures of socket (sockaddr)
#include <netinet/in.h> //defines constants and structures (sockaddr_in)
#include <stdlib.h> //define var_types, macros, funcs etc. (atio())
#include <unistd.h> //read, write, close

void error(const char *mssg)
{
	perror(mssg);
	exit(1);
}

int main(int argc, char* argv[])
{
	//argv => portNumber
	//if user do not pass port number
	if(argc<2)
		error("Port number must be entered.");
	int PORT = atoi(argv[1]);
	
	//structure of server and client addr
	struct sockaddr_in server_addr, client_addr;
	
	//creating server socket
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	//specifying server address
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	
	//binding
	if( bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr)) != 0);
		error("[BINDING FAILED]");
		
	//listen
	listen(server_socket, 5);
	
	//accept any connection and save the client address
	int client_socket = accept(server_socket, (struct sockaddr*) &client_addr, sizeof(client_addr));
	
	char buffer[256]; //mssg 
	while(1)
	{
		/*Client will send message first*/
		bzero(buffer, 256); //clean previous mssgs
		
		//receive data from client
		read(client_socket, buffer, 256);
		printf("[CLIENT]: %s\n", buffer);
		
		bzero(buffer, 256); //clean previous mssgs
		fgets(buffer, 256, stdin);//input from console
		
		//send mssg to client
		write(client_socket, buffer, sizeof(buffer));
		
		//logic when to quit connection
		if( strcmp("quit", buffer, 4) == 0 )
			break;
	}
	
	close(client_socket);
	close(server_socket);
	
	return 0;

}
