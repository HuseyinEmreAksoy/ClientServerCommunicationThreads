#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>

struct mesg_buffer {
    long mesg_type;
    char mesg_text[100];
    char senderId [100];
    char receiveId[100];
} message;

typedef struct{
    int msgid;
    key_t key;
    struct mesg_buffer* message;
} threadP;

void* communication(void* arg);

int cript(char* msg){
    int i = 0;
    int criptedNo = 0;
    for(i = 0; i <  strlen(msg); i++){
        criptedNo += (i + 1) * (msg[i] - '0');
    }
    return criptedNo;
}
  
int main()
{
    key_t key;
    int msgid;
    key = ftok("./readme.txt", 65);

    int running=1;
    msgid = msgget(key, 0666 | IPC_CREAT);

    while(running)
    {
        msgrcv(msgid, &message, sizeof(message), 1, 0);
        threadP* args = (threadP*) malloc(sizeof(threadP));
        args->msgid = msgid;
        args->key = key;
        args->message = &message;
        pthread_t p1;
        pthread_create(&p1, NULL, &communication, (void*)args );
        
        if(strncmp(message.mesg_text,"end",3)==0)
            running=0;
        
    }
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}
void* communication(void* arg){
    threadP* params = (threadP*)arg;
    long port = (long)cript(params->message->senderId) + 1;
    while (1) {
        msgrcv(params->msgid, params->message, sizeof(*(params->message)), port, 0);
        printf("Message port is %ld ",port);
        printf("Message is: %s\n",params->message->mesg_text);
        params->message->mesg_type = (long)cript(params->message->receiveId);
        msgsnd(params->msgid, params->message, sizeof(*(params->message)), 0);
    }
}
