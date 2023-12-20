#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in serv_addr;

    // Check the command-line arguments
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <server or client> <IP address> <port>\n", argv[0]);
        exit(1);
    }

    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        error("Error opening socket");

    // Initialize server address structure
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    // If in server mode, bind to INADDR_ANY; otherwise, use the specified IP address
    if (strcmp(argv[1], "server") == 0)
    {
        serv_addr.sin_addr.s_addr = INADDR_ANY;
    }
    else if (inet_pton(AF_INET, argv[2], &serv_addr.sin_addr) <= 0)
    {
        fprintf(stderr, "Invalid IP address\n");
        exit(1);
    }

    // Use the specified port
    serv_addr.sin_port = htons(atoi(argv[3]));

    if (strcmp(argv[1], "server") == 0)
    {
        // Server mode
        // Bind the socket
        if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
            error("Error on binding");

        printf("Server: Waiting for a connection...\n");

        while (1)
        {
            // Receive message from the client
            socklen_t cli_len = sizeof(serv_addr);
            memset(buffer, 0, sizeof(buffer));
            if (recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&serv_addr, &cli_len) < 0)
                error("Error on receive");

            printf("Client: %s", buffer);

            // If the message is "bye", end the chat
            if (strcmp(buffer, "bye\n") == 0)
            {
                printf("Connection closed.\n");
                break;
            }

            // Get user input and send the message to the client
            printf("You: ");
            fgets(buffer, sizeof(buffer), stdin);
            if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&serv_addr, cli_len) < 0)
                error("Error on send");

            // If the message is "bye", end the chat
            if (strcmp(buffer, "bye\n") == 0)
            {
                printf("Connection closed.\n");
                break;
            }
        }
    }
    else if (strcmp(argv[1], "client") == 0)
    {
        // Client mode
        while (1)
        {
            // Get user input and send the message to the server
            printf("You: ");
            fgets(buffer, sizeof(buffer), stdin);
            if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
                error("Error on send");

            // If the message is "bye", end the chat
            if (strcmp(buffer, "bye\n") == 0)
            {
                printf("Connection closed.\n");
                break;
            }

            // Receive message from the server
            memset(buffer, 0, sizeof(buffer));
            socklen_t serv_len = sizeof(serv_addr);
            if (recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&serv_addr, &serv_len) < 0)
                error("Error on receive");

            printf("Server: %s", buffer);

            // If the message is "bye", end the chat
            if (strcmp(buffer, "bye\n") == 0)
            {
                printf("Connection closed.\n");
                break;
            }
        }
    }
    else
    {
        fprintf(stderr, "Invalid argument. Use 'server' for server or 'client' for client.\n");
        exit(1);
    }

    // Close the socket
    close(sockfd);

    return 0;
}
