#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_LEN 100

char *close_client = "Close client";
int skt;
struct sockaddr_in server;
struct sockaddr_in client;

//Send command to the server
void sendMessageServer()
{
    char buffer[MAX_LEN];
    char* newBuff = NULL;
    int i, nrecv;
    unsigned int brecv = 0;
    for (;;)
    {
        bzero(buffer, sizeof(buffer));
        printf("Enter the command: ");
        i = 0;
        while ((buffer[i++] = getchar()) != '\n')
            ;
        buffer[i - 1] = '\0';

        //Send the buffer
        if (write(skt, buffer, sizeof(buffer)) < 0)
        {
            perror("write");
            close(skt);
            exit(1);
        }

        if (strcmp(close_client, buffer) == 0)
        {
            printf("The connection has finished");
            close(skt);
            exit(1);
        }
        bzero(buffer, sizeof(buffer));

        nrecv = recv(skt, buffer, sizeof(buffer), 0);
        // printf("%s", buffer);
        if(nrecv < 0)
        {
            perror("recv");
            exit(1);
        }
        // else if(nrecv == MAX_LEN)
        // {
           
        printf("%s",buffer);
        while(nrecv >= MAX_LEN)
        {
            if((nrecv = recv(skt, buffer, sizeof(buffer), 0)) > 0)
            {
                printf("%s", buffer);
            }
            else
            {
                break;
            }
            
        }
        
        // {
        
        // }
        printf("\n");
            
        // }
        
        // if(nrecv < 0)
        // {
        //     
        // }
        // printf("%s\n", buffer);

        // if ((nrecv = recv(skt, buffer, sizeof(buffer), 0)) < 0)
        // {
        //     perror("recv");
        //     exit(1);
        // }
        // printf("%d\n", nrecv);
        // printf("Server reply: \n%s\n", newBuff);
    }
}

void signalHandler(int signal)
{
    printf("\nClosing the socket\n");
    close(skt);
    exit(1);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, signalHandler);

    if (argc <= 1)
    {
        printf("HELP: Must add the port number\n");
        return 1;
    }

    if ((skt = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1]));
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    bzero(&client.sin_zero, 8);

    // skt = createClientSocket(argv[1]);
    if (connect(skt, (struct sockaddr *)&server, sizeof(server)) < 0)
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