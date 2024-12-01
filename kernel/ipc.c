#include "types.h"
#include "param.h"
#include "message.h"
#include "defs.h"
#include "spinlock.h"
#include "proc.h"

struct message_queue msg_queue;

void init_message_queue(void) {
    initlock(&msg_queue.lock, "msg_queue");
    msg_queue.head = 0;
    msg_queue.tail = 0;
    msg_queue.count = 0;
}

uint64 sys_send(void) {
    int pid;     // Destinatario (opcional según diseño)
    char *msg;   // Mensaje desde el usuario

    // Leer argumentos de la llamada al sistema
     if (argint(0, &pid) < 0 || argstr(1, msg, MAX_MSG_SIZE) < 0) {
        return -1;
    }

    acquire(&msg_queue.lock);

    // Verificar si la cola está llena
    if (msg_queue.count >= MAX_MESSAGES) {
        release(&msg_queue.lock);
        return -1; // Cola llena
    }

    // Crear un nuevo mensaje
    message new_msg;
    new_msg.sender_pid = myproc()->pid; // Obtener PID del remitente
    safestrcpy(new_msg.content, msg, sizeof(new_msg.content));

    // Insertar el mensaje en la cola
    msg_queue.messages[msg_queue.tail] = new_msg;
    msg_queue.tail = (msg_queue.tail + 1) % MAX_MESSAGES;
    msg_queue.count++;

    wakeup(&msg_queue); // Despertar cualquier lector bloqueado
    release(&msg_queue.lock);

    return 0; // Éxito
}

uint64 sys_receive(void) {
    char buffer[MAX_MSG_SIZE]; // Dirección del buffer del usuario

    // Leer argumento de la llamada al sistema
    if (argstr(0, buffer, MAX_MSG_SIZE) < 0) {
        return -1;
    }

    acquire(&msg_queue.lock);

    // Bloquearse si la cola está vacía
    while (msg_queue.count == 0) {
        sleep(&msg_queue, &msg_queue.lock);
    }

    // Extraer el mensaje más antiguo
    message received_msg = msg_queue.messages[msg_queue.head];
    msg_queue.head = (msg_queue.head + 1) % MAX_MESSAGES;
    msg_queue.count--;

    release(&msg_queue.lock);

    // Copiar contenido al buffer del usuario
    if (copyout(myproc()->pagetable, (uint64)buffer, received_msg.content, sizeof(received_msg.content)) < 0) {
        return -1;
    }

    return received_msg.sender_pid; // Retorna el PID del remitente
}
