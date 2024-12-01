// message.h
#ifndef MESSAGE_H
#define MESSAGE_H

#include "spinlock.h"

#define MAX_MESSAGES 64
#define MSG_SIZE 128

typedef struct message {
    int sender_pid;      // ID del proceso remitente
    char content[MSG_SIZE]; // Contenido del mensaje
} message;

typedef struct message_queue {
    struct spinlock lock;             // Spinlock para sincronización
    message messages[MAX_MESSAGES];   // Array de mensajes
    int head;                         // Índice del primer mensaje
    int tail;                         // Índice del próximo espacio disponible
    int count;                        // Número actual de mensajes
} message_queue;

#endif // MESSAGE_H
