#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>

#define BUFFER_SIZE 1024
#define MARK_SIZE 78

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void init_serv_addr(struct sockaddr_in* serv_addr, int mode, const char* cli_addr, int port)
{
    memset(serv_addr, 0, sizeof(*serv_addr));
    serv_addr -> sin_family = AF_INET;

    // If in server mode, bind to INADDR_ANY; otherwise, use the specified IP address
    if (mode)
    {
        serv_addr -> sin_addr.s_addr = INADDR_ANY;
    }
    else if (inet_pton(AF_INET, cli_addr, &(serv_addr -> sin_addr)) <= 0)
    {
        fprintf(stderr, "Invalid IP address\n");
        exit(1);
    }

    serv_addr -> sin_port = htons(port);
}

int main(int argc, char *argv[]) {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in serv_addr, addr_to_bind;

    // Check the command-line arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <your name>:<password>@ip_address:<port>\n", argv[0]);
        exit(1);
    }
    const char* credentials = argv[1];
    int port;
    char user_name[256], password[256], ip_address[256]; 
    if (sscanf(credentials, "%255[^:]:%255[^@]@%255[^:]:%d", user_name, password, ip_address, &port) < 4) {
        error("Failed to parse infomation");
        exit(1);
    }else{
        char password_mask[256];
        for(int i = 0; i < strlen(password); ++i)
            password_mask[i] = '*';
        password_mask[strlen(password)] = '\0';
        printf("--- INFO ---\nuser name: %s\npassword: %s\nip address: %s\nport: %d\n------------\n", user_name, password_mask, ip_address, port);
    }

    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        error("Error opening socket");

    // initialize ip addresses
    init_serv_addr(&addr_to_bind, 1, "*", port);

    init_serv_addr(&serv_addr, 0, ip_address, port);

    // Get user name
    char *name = user_name;

    if (bind(sockfd, (struct sockaddr *)&addr_to_bind, sizeof(addr_to_bind)) < 0)
            error("Error on binding");

    time_t timer;
    char mark_msg[MARK_SIZE * 4];
    /*
    char* s = "─";
    for(int i = 0; i < MARK_SIZE * 4; i += 4){
        mark_msg[i] = s[0];
        mark_msg[i+1] = s[1];
        mark_msg[i+2] = s[2];
        mark_msg[i+3] = s[3];
    }
    */
    // to support gcc on macOS
    sprintf(mark_msg, "───────────────────────────────────────────────────────────────────────────────");
    // Fork the process to create a child process for receiving messages
    pid_t pid = fork();

    if (pid == -1) {
        error("Error on fork");
    } else if (pid == 0) {
        // Child process: Receive messages
        char _password[256];
        while (1) {
            memset(buffer, 0, sizeof(buffer));
            socklen_t serv_len = sizeof(serv_addr);
            if (recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&serv_addr, &serv_len) < 0)
                error("Error on receive\n");
            if(sscanf(buffer, "PASSWORD %255[^$]$", _password) != 1)
                continue;
            if(strcmp(password, _password))
                continue;    
            time(&timer);
            printf("\r┌%s┐\n %s %s└%s┘\n", mark_msg, ctime(&timer), buffer + strlen(_password) + 10, mark_msg);
        }
    } else {
        // Parent process: Send messages
        char raw_msg[BUFFER_SIZE - strlen(name) - 5];
        while (1) {
            // Get user input and send the message to the other computer
            fgets(raw_msg, sizeof(raw_msg), stdin);
            if(strcmp(raw_msg, "/exit\n") == 0)
                break;
            sprintf(buffer, "PASSWORD %s$%s\n ---\n %s", password, name, raw_msg);
            time(&timer);
            printf("\033[A\r┌%s┐\n %s %s\n ---\n %s└%s┘\n", mark_msg, ctime(&timer), name, raw_msg, mark_msg);
            if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
                error("Error on send");
        }
        if(kill(pid, SIGKILL))
            error("Fail to kill message receiver process");
    }

    close(sockfd);

    return 0;
}
