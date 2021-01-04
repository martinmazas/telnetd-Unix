#include <stdio.h>
#include <string.h>	
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

char *close_client = "Close client";

int createClientSocket(char *port)
{
    struct sockaddr_in server;
    int portInt = atoi(port);

    //Create the socket
    int socketInt = socket(AF_INET, SOCK_STREAM, 0);
    if(socketInt == -1) 
    {
        printf("Could not create a socket\n");
    }

    //sockaddr_in info
    server.sin_family = AF_INET;
    server.sin_port = htons(portInt);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    //Connect
    if(connect(socketInt, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf("Cannot connect\n");
        exit(-1);
    }
    else
    {
        printf("Connected\n");
    }
    
}

int main(int argc, char * argv[])
{
    
    
    return 0;
}