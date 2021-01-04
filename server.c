#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>



int skt;
char *close_client = "Close client";

int createServerSocket(char *port) 
{
    struct sockaddr_in server;
    int portInt = atoi(port);

    //Create the socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket == -1)
    {
        printf("Could not create a socket\n");
        exit(-1);
    }

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

    printf("Server bind in port %d\n", portInt);
    return serverSocket;
}

int acceptConnection(int socket, fd_set *readfds)
{
    struct sockaddr_in client;
    int clientSocket;

    //accept connection
    clientSocket = accept(socket, (struct sockaddr *)&client, sizeof(struct sockaddr_in));
    if(clientSocket == -1)
    {
        printf("Cannot accept the connection");
        exit(-1);
    }

    //Receive message from client
    

}
int main(int argc, char * argv[])
{
    if( argc <= 1)
    {
        printf("HELP: Must add the port number\n");
        exit(-1);
    }

    char * port = argv[1];
    skt = createServerSocket(port);

    if(socket != -1)
    {
        printf("There is a socket");
        // while(1)
        // {
        //     //Need to add listener
        // }
    }
    return 0;
}