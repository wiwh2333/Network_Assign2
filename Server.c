/*
	C socket server example, handles multiple clients using threads
*/

#include<stdio.h>
#include<string.h>	//strlen
#include<stdlib.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#include<pthread.h> //for threading , link with lpthread
#include <dirent.h> //For directories

//the thread function
void *connection_handler(void *);

int main(int argc , char *argv[])
{
	int socket_desc , client_sock , c , *new_sock;
	struct sockaddr_in server , client;
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
    //server.sin_addr.s_addr = inet_addr("10.224.76.120");
	server.sin_port = htons( 8888 );
	
	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		//print the error message
		perror("bind failed. Error");
		return 1;
	}
	puts("bind done");
	
	//Listen
	listen(socket_desc , 3);
	
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	
	
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
	{
		puts("Connection accepted");
		
		pthread_t sniffer_thread;
		new_sock = malloc(1);
		*new_sock = client_sock;
		
		if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
		{
			perror("could not create thread");
			return 1;
		}
		
		//Now join the thread , so that we dont terminate before the thread
		//pthread_join( sniffer_thread , NULL);
        //Send some messages to the client
	    
	
	    // char *message = "Greetings! I am your connection handler\nNow type something and i shall repeat what you type \n";
	    // send(client_sock , message , strlen(message),0);
		puts("Handler assigned");
	}
	
	if (client_sock < 0)
	{
		perror("accept failed");
		return 1;
	}
	
	return 0;
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
	//Get the socket descriptor
	int sock = *(int*)socket_desc;
	int read_size, length, error =200;
	char *message , client_message[2000], ret[2000], file_length[20];
    char *RequestMethod, *RequestURL, *RequestVersion;
	DIR *dir; FILE *file;
    struct dirent *entry;
	
	
	//Receive a message from client
	while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
	{
		//Send the message back to client
		//send(sock , client_message , strlen(client_message),0);
        char *token = strtok(client_message, " \t");
        RequestMethod = token;
        token = strtok(NULL, " \t");
        RequestURL = token;
        token = strtok(NULL, " \t");
        RequestVersion = token;
        token = strtok(NULL, " \t");
        dir = opendir(RequestURL);
        if (dir == NULL) {perror ("Can't find directory");}
        strcat(RequestURL, "/index.html");
        file = fopen(RequestURL, "rb");
        fseek(file, 0, SEEK_END); length = ftell(file); fseek(file, 0 , SEEK_SET);
        message = (char *)malloc(length+1);
        fread(message, length, 1, file);
        message[length] = '\0';
        printf("METHOD: %s, URL %s, VERSION%s\n", RequestMethod, RequestURL, RequestVersion);
        strcat(RequestVersion, "200 OK\r\n");
        strcat(RequestVersion, "Content-Type: index.html\r\n");
        sprintf(file_length, "%d", length);
        strcat(RequestVersion, "Content-Length: ");
        strcat(RequestVersion, file_length);
        strcat(RequestVersion, "\r\n\r\n");
        strcat(RequestVersion, message);
        printf("HERE:%s\n", RequestVersion);
        send(sock , RequestVersion , strlen(RequestVersion),0);
        
        
	}

	if(read_size == 0)
	{
		puts("Client disconnected");
		fflush(stdout);
	}
	else if(read_size == -1)
	{
		perror("recv failed");
	}
		
	//Free the socket pointer
	free(socket_desc);
	
	return 0;
}