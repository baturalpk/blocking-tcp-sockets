//
// Created by Baturalp KIZILTAN on 7.10.2021.
//

#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "model.h"

void generate_new_message(struct message *new_msg, char *sender, char *content)
{
    strcpy(new_msg->sender, sender);
    strcpy(new_msg->content, content);

    time_t t_now;
    time(&t_now);
    strcpy(new_msg->timestamp, ctime(&t_now));
}

void pretty_print_message(struct message *msg)
{
    printf("\n-----\nSENDER: %s,\nCONTENT: %s,\nTIME: %s\n-----\n",
           msg->sender,
           msg->content,
           msg->timestamp
    );
}

int send_message_as_whole(int connfd, struct message *new_msg, size_t sizeof_new_msg)
{
    long bytes_sent = 0;
    long total_bytes_left = (long) sizeof_new_msg;
    long total_bytes_sent = 0;

    // Handle partial sending
    while (total_bytes_sent < sizeof_new_msg)
    {
        bytes_sent = send(connfd, new_msg + total_bytes_sent, total_bytes_left, 0);

        if (bytes_sent == -1)
        {
            perror("cannot send message, ...");
            return -1;
        }

        total_bytes_left -= bytes_sent;
        total_bytes_sent += bytes_sent;
    }

    return 0;
}
