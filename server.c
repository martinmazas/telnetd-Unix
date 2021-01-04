#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>



// int socket;
char *close_client = "Close client";

int createServerSocket(char *port) {
    struct sockaddr_in server;
    int portInt = atoi(port);

    //Create the socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket == -1)
    {
        printf("Could not create a socket\n");
        exit(-1);
    }
    // printf("Socket created");

    //sockaddr_in info
    server.sin_family = AF_INET;
    server.sin_port = htons(portInt);
    server.sin_addr.s_addr = INADDR_ANY;

    //Bind
    if((bind(serverSocket, (struct sockaddr *)&server, sizeof(server))) < 0)
    {
        printf("Bind error\n");
        exit(-1);
    }

    //Listen
    if(listen(serverSocket, 3) == -1)
    {
        printf("Listen error\n");
        exit(-1);
    }

    
}

int main(int argc, char * argv[])
{
    
    return 0;
}