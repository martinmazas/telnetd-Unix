#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>


#define QUEUE_LEN 20
#define MAX_LEN 30

int skt;
int total_clients = 0;
int client_socks[QUEUE_LEN];
char *close_client = "Close client";

//End the connection with the client 
void endConnection(int client, int client_number)
{
    printf("The connection with client %d was ended\n", client);
    close(client);
    client_socks[client_number] = 0;
}


//Create a server side socket on given port
int createServerSocket(char *port) 
{
    struct sockaddr_in server;
    int portInt = atoi(port);

    //Create the socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket < -1)
    {
        perror("socket");
        return 1;
    }

    //sockaddr_in info
    server.sin_family = AF_INET;
    server.sin_port = htons(portInt);
    server.sin_addr.s_addr = INADDR_ANY;

    //Bind
    if((bind(serverSocket, (struct sockaddr *)&server, sizeof(server))) < 0)
    {
        perror("bind");
        return 1;
    }

    //Listen
    if(listen(serverSocket, QUEUE_LEN) < 0)
    {
        perror("listen");
        return 1;
    }

    printf("Server bind in port %d\n", portInt);
    return serverSocket;
}

//Accept connections from all the clients
int acceptConnection(int socket, fd_set *readfds)
{
    struct sockaddr_in clientIn;
    int sockaddr_size = sizeof(struct sockaddr_in);
    int clientSocket;
    int max_sd, sd, activity, new_socket;

    //Clear fd_set
    FD_ZERO(readfds);

    //Add socket to set
    FD_SET(socket, readfds);
    max_sd = socket;

    for(int i = 0; i < total_clients; i++ )
    {
        sd = client_socks[i];

        if(sd > 0)
        {
            FD_SET(sd, readfds);
        }

        if(sd > max_sd)
        {
            max_sd = sd;
        }
    }

    activity = select(max_sd + 1, readfds, NULL, NULL, NULL);
    if((activity < 0) && (errno != EINTR))
    {
        printf("select error");
    }

    //if something happend in socket then its an incoming connection
    if(FD_ISSET(socket, readfds))
    {
        //accept connection
        if((new_socket = accept(socket, (struct sockaddr *)&clientIn, &sockaddr_size)) < 0)
        {
            perror("accept");
            return 1;
        }
        client_socks[total_clients++] = new_socket;
        printf("New client connection from IP: %s in port %d\n", inet_ntoa(clientIn.sin_addr), ntohs(clientIn.sin_port));

    }

    return max_sd;
}

// Execute the wanted command
int runCommand(int client, char *message, int message_size)
{
    FILE *fp;
    char buffer[MAX_LEN], command[MAX_LEN];
    char *str = NULL, *tmp = NULL;
    int str_size, size =1;


    pid_t pid = fork();
    if(pid < 0)
    {
        perror("fork");
        return 1; 
    }
    if(pid == 0)
    {
        //Command format
        sprintf(command, "%s 2>&1 ", message);
        fp = popen(command, "r");
        while(fgets(buffer, sizeof(buffer), fp) != NULL)
        {
            str_size = strlen(buffer);
            tmp = realloc(str, str_size + size);
            if(tmp == NULL)
            {
                perror("buffer size");
                return 1;
            }
            else
            {
                str = tmp;
            }
            strcpy(str + size - 1, buffer);
            size += str_size;
        }
        send(client, str, size - 1, 0);
        pclose(fp);
        return 0;
    }
}

void stringHandler(fd_set *readfds)
{
    int client_socket, data_size;
    char buffer[MAX_LEN]; 

    for(int i = 0; i < total_clients; i++)
    {
        client_socket = client_socks[i];
        if(FD_ISSET(client_socket, readfds))
        {
            data_size = recv(client_socket, buffer, MAX_LEN, 0);

            if(data_size == 0)
            {
                endConnection(client_socket, i);
            }

            if(data_size)
            {
                buffer[data_size - 2] ='\0';
            }

            if(strcmp(close_client, buffer) == 0)
            {
                endConnection(client_socket, i);
            }
            else
            {
               runCommand(client_socket, buffer, data_size);
            }
            
        }   
    }
}

//Closing client connections when the server is closed
void signalHandler(int signal)
{
    int closed_connections = 0;
    for(int i = 0; i<total_clients; i++ )
    {
        if(client_socks[i] != 0)
        {
            closed_connections++;
            close(client_socks[i]);
        }
    }
    printf("Total number of closed clients: %d\n", closed_connections);
    close(skt);
    printf("Closed server\n");
    exit(1);
}




int main(int argc, char * argv[])
{
    if( argc <= 1)
    {
        printf("HELP: Must add the port number\n");
        return 1;
    }

    fd_set readfds;
    
    char * port = argv[1];
    skt = createServerSocket(port);

    if(skt != -1)
    {
        signal(SIGINT, signalHandler);
        signal(SIGABRT,signalHandler);
        while(1)
        {
            acceptConnection(skt, &readfds);
            stringHandler(&readfds);
        }
    }

    close(skt);
    return 0;
}