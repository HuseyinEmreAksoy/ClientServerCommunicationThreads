#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include <math.h>
#include <pthread.h>

#define MAX 55

void* revice(void* args);

struct mesg_buffer {
    long mesg_type;
    char mesg_text[100];
    char senderId [100];
    char receiveId[100];
} message;

typedef struct{
    int msgid;
    key_t key;
    char reelSenderId [100];
    struct mesg_buffer* message;
} threadP;

int cript(char* msg);
void* receive(void* args);
  
int main(int argc, char *argv[])
{
    char reelSenderId [100];
    strcpy(reelSenderId,argv[1]);
    key_t key;
    int msgid;
    int running=1;
    char str[100];
    key = ftok("./readme.txt", 65);
    msgid = msgget(key, 0666 | IPC_CREAT);
    
    strcpy(message.senderId,argv[1]);
    long port = (long)cript(argv[1]) + 1;
    message.mesg_type = 1;
    strcpy(message.mesg_text,argv[1]);
    msgsnd(msgid, &message, sizeof(message), 0);

    threadP* param = (threadP*) malloc(sizeof(threadP));
    param->msgid = msgid;
    param->key = key;
    param->message = &message;
    strcpy(param->reelSenderId,reelSenderId);

    pthread_t pRec;
    pthread_create(&pRec, NULL, &receive, (void*)param);

    while(running)
    {
        printf("\nWrite receiver name: ");
        fgets(str,MAX,stdin);
        if (str[strlen(str) - 1] == '\n')
            str[strlen(str) - 1] = '\0';
    	strcpy(message.receiveId,str);
        printf("\nWrite text : ");
        fgets(message.mesg_text,MAX,stdin);

        if (message.mesg_text[strlen(message.mesg_text) - 1] == '\n')
            message.mesg_text[strlen(message.mesg_text) - 1] = '\0';
        message.mesg_type = port;
        msgsnd(msgid, &message, sizeof(message), 0);
    
        if(strncmp(message.mesg_text,"end",3)==0)
            running=0;
    }
    return 0;
}

void* receive(void* args){
    threadP* paramReceive = (threadP*)args;
    while (1) {
        msgrcv(paramReceive->msgid, paramReceive->message, sizeof(*(paramReceive->message)), (long)cript(paramReceive->reelSenderId), 0);
        printf("\nText: %s \n", paramReceive->message->mesg_text);
    }
}

int cript(char* msg){
    int i = 0;
    int criptedNo = 0;
    for(i = 0; i <  strlen(msg); i++){
        criptedNo += (i + 1) * (msg[i] - '0');
    }
    return criptedNo;
}
