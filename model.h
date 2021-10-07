//
// Created by Baturalp KIZILTAN on 6.10.2021.
//

#ifndef TCP_CLIENT_SERVER_MODEL_H
#define TCP_CLIENT_SERVER_MODEL_H

struct message
{
    char sender[1024];
    char content[1024];
    char timestamp[1024];
};

void generate_new_message(struct message *, char *, char *);
void pretty_print_message(struct message *);
int send_message_as_whole(int, struct message *, size_t);

#endif //TCP_CLIENT_SERVER_MODEL_H
