#include <stdio.h>
#include <string.h>	
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_LEN 30

char *close_client = "Close client";
int skt;
struct sockaddr_in server;
struct sockaddr_in client;


//Create client side socket
int createClientSocket(char *port)
{
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
    bzero(&client.sin_zero, 8);

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


//Send command to the server
void sendMessageServer(int socket) 
{
    char buffer[MAX_LEN], server_reply[MAX_LEN];

    while(1)
    {
        printf("Enter the command: ");
        int i = 0;
        while((buffer[i++] = getchar()) != '\n');
        buffer[i-1] = '\0';

        //Send the buffer
        if(write(socket, buffer, sizeof(buffer)) < 0)
        {
            perror("write");
            close(socket);
            exit(1);
        }

        if(strcmp(close_client, buffer) == 0)
        {
            printf("The connection is finished");
            close(socket);
            exit(1);
        }

        if(recv(socket, server_reply, MAX_LEN, 0) < 0)
        {
            perror("recv");
            exit(1);
        }

        puts("Server reply :");
        puts(server_reply);
    }

}

void signalHandler(int signal)
{
    printf("\nClosing the socket\n");
    close(skt);
    exit(1);
}

int main(int argc, char * argv[])
{
    signal(SIGINT, signalHandler);
    if(argc <= 1)
    {
        printf("HELP: Must add the port number\n");
        return 1;
    }

    char *port = argv[1];
    skt = createClientSocket(port);
    sendMessageServer(skt);
    
    close(skt);
    return 0;
}