//
// server.c
//
// Created by Baturalp KIZILTAN on 6.10.2021.
//

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "model.h"

#define INFINITE 1
#define SIN_PORT 4444

void fatal(const char *);
void configure_socketaddr(struct sockaddr_in*);

int main(void)
{
    struct sockaddr_in sockaddr;
    configure_socketaddr(&sockaddr);

    printf("creating socket file desc.\n");
    int sockfd = socket(
        AF_INET,        // AF_INET vs PF_INET confusion: https://stackoverflow.com/a/6737450
        SOCK_STREAM,    // For TCP/IP in this case: https://www.ibm.com/docs/en/aix/7.1?topic=protocols-socket-types
        IPPROTO_TCP
    );

    if (sockfd == -1)
    {
        fatal("socket file descriptor creation failed\n");
    }

    printf("binding socket\n");
    if (bind(sockfd, (const struct sockaddr *) &sockaddr, sizeof sockaddr) == -1)
    {
        close(sockfd);
        fatal("socket binding failed\n");
    }

    printf("setting up socket listener\n");
    if (listen(sockfd, SOMAXCONN) == -1)
    {
        close(sockfd);
        fatal("socket listening failed\n");
    }

    while (INFINITE)
    {
        printf("setting up to accept connections\n");
        int connfd = accept(sockfd, NULL, NULL);

        if (connfd == -1)
        {
            perror("connection file descriptor creation error, ...\n");
            continue;
        }

        // Read message
        printf("reading message\n");
        struct message buffer;
        if (recv(connfd, &buffer, sizeof (struct message), 0) == -1)
        {
            perror("cannot read received message, ...\n");
            goto SHUTDOWN;
        }
        pretty_print_message(&buffer);

        if (strcmp(buffer.content, "EXIT") == 0)
        {
            printf("exiting...\n");
            shutdown(connfd, SHUT_RDWR);
            close(connfd);
            break;
        }

        // Send message
        printf("writing message\n");
        struct message new_msg;
        generate_new_message(&new_msg, "Server", "This message's generated by server.");

        if (send_message_as_whole(connfd, &new_msg, sizeof new_msg) == -1)
        {
            perror("message sending failed, ...\n");
            goto SHUTDOWN;
        }

        SHUTDOWN: { // try to shut down both sides of the connection
            if (shutdown(connfd, SHUT_RDWR) == 0)
            {
                close(connfd);
            }
            else
            {
                close(connfd);
                close(sockfd);
                fatal("connection shutdown failed\n");
            }
        }
    }

    close(sockfd);
    return 0;
}

void fatal(const char *msg)
{
    perror(msg);
    exit(1);
}

void configure_socketaddr(struct sockaddr_in* server_saddr)
{
    memset(server_saddr, 0, sizeof *server_saddr); // Initialize with 0

    server_saddr->sin_family      = AF_INET;            // IPv4 address family
    server_saddr->sin_port        = htons(SIN_PORT);
    server_saddr->sin_addr.s_addr = htonl(INADDR_ANY);  // Any available interfaces for binding
    /* my_saddr->sin_addr.s_addr = htonl(INADDR_LOOPBACK); // Localhost via loopback interface */
}
