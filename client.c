#include <stdio.h>
#include <string.h>	
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_LEN 1024

char *close_client = "Close client";
int skt;
struct sockaddr_in server;
struct sockaddr_in client;


//Send command to the server
void sendMessageServer() 
{
    char buffer[MAX_LEN];

    int i, nrecv;
    for(;;)
    {
        bzero(buffer, sizeof(buffer));
        printf("Enter the command: ");
        i = 0;
        while((buffer[i++] = getchar()) != '\n');
        buffer[i-1] = '\0';

        //Send the buffer
        if(write(skt, buffer, sizeof(buffer)) < 0)
        {
            perror("write");
            close(skt);
            exit(1);
        }

        if(strcmp(close_client, buffer) == 0)
        {
            printf("The connection is finished");
            close(skt);
            exit(1);
        }
        bzero(buffer, sizeof(buffer));

        // read(skt, buffer, sizeof(buffer));
        if ((nrecv = recv(skt, buffer, sizeof(buffer), 0)) < 0)
        {
            perror("recv");
            exit(1);
        }
	    // printf("%s\n", buffer);
        printf("Server reply: \n%s\n", buffer);

        // if(recv(socket, server_reply, MAX_LEN, 0) < 0)
        // {
        //     perror("recv");
        //     exit(1);
        // }

        // puts("Server reply :");
        // puts(server_reply);
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

    if((skt = socket(AF_INET,SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1]));
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    bzero(&client.sin_zero, 8);

    // skt = createClientSocket(argv[1]);
    if(connect(skt, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect");
        return 1;
    }
    else
    {
        printf("Connected\n");
    }

    sendMessageServer();
    close(skt);
    return 0;
}