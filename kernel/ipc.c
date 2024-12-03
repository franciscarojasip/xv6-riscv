#include "types.h"
#include "param.h"
#include "message.h"
#include "defs.h"
#include "spinlock.h"
#include "proc.h"
#include <stddef.h>

struct message_queue msg_queue;

void init_message_queue(void) {
    initlock(&msg_queue.lock, "msg_queue");
    msg_queue.head = 0;
    msg_queue.tail = 0;
    msg_queue.count = 0;
}

uint64 sys_send(void) {
    int pid;
    char msg[MSG_SIZE]; // Buffer para el mensaje

    if (argint(0, &pid) < 0 || argstr(1, msg, MSG_SIZE) < 0) {
        return -1; // Error en argumentos
    }

    acquire(&msg_queue.lock);

    if (msg_queue.count >= MAX_MESSAGES) {
        release(&msg_queue.lock);
        return -1; // Cola llena
    }

    message new_msg;
    new_msg.sender_pid = myproc()->pid;
    safestrcpy(new_msg.content, msg, sizeof(new_msg.content));

    msg_queue.messages[msg_queue.tail] = new_msg;
    msg_queue.tail = (msg_queue.tail + 1) % MAX_MESSAGES;
    msg_queue.count++;

    wakeup(&msg_queue); // Despertar lectores bloqueados
    release(&msg_queue.lock);

    return 0; // Éxito
}

uint64 sys_receive(void) {
    char *buffer; // Puntero al buffer del usuario

    if (argaddr(0, (uint64 *)&buffer) < 0) {
        return -1; // Error en argumentos
    }

    acquire(&msg_queue.lock);

    while (msg_queue.count == 0) {
        sleep(&msg_queue, &msg_queue.lock); // Bloquearse si no hay mensajes
    }

    message received_msg = msg_queue.messages[msg_queue.head];
    msg_queue.head = (msg_queue.head + 1) % MAX_MESSAGES;
    msg_queue.count--;

    release(&msg_queue.lock);

    if (copyout(myproc()->pagetable, (uint64)buffer, received_msg.content, sizeof(received_msg.content)) < 0) {
        return -1; // Error en copyout
    }

    return received_msg.sender_pid; // Retornar el PID del remitente
}
