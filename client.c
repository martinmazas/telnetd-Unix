#include <stdio.h>
#include <string.h>	
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_LEN 30

char *close_client = "Close client";

int createClientSocket(char *port)
{
    struct sockaddr_in server;
    int portInt = atoi(port);

    //Create the socket
    int socketInt = socket(AF_INET, SOCK_STREAM, 0);
    if(socketInt == -1) 
    {
        perror("socket");
        return 1;
    }

    //sockaddr_in info
    server.sin_family = AF_INET;
    server.sin_port = htons(portInt);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    //Connect
    if(connect(socketInt, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect");
        return 1;
    }
    else
    {
        printf("Connected\n");
    }

    return socketInt;
}


void sendMessageServer(int socket) 
{
    char buffer[MAX_LEN], server_reply[MAX_LEN];

    while(1)
    {
        printf("Enter a message: ");
        scanf("%s", buffer);

        //Send the buffer
        if(send(socket, buffer, strlen(buffer), 0) < 0)
        {
            perror("send");
            return;
        }

        if(recv(socket, server_reply, MAX_LEN, 0) < 0)
        {
            perror("recv");
            return;
        }

        puts("Server reply :");
        puts(server_reply);


    }

}

int main(int argc, char * argv[])
{
    int socket;
    if(argc <= 1)
    {
        printf("HELP: Must add the port number\n");
        return 1;
    }

    char *port = argv[1];
    socket = createClientSocket(port);
    sendMessageServer(socket);
    
    close(socket);
    return 0;
}