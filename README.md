## Informe Tarea 4


### Funcinamiento y lógica

Este sistema utiliza una cola de mensajes dentro del kernel para permitir que los procesos envíen y reciban mensajes de forma sincronizada.
Cada mensaje contiene el PID del remitente y un contenido, que se almacena en una cola circular protegida por un spinlock para evitar accesos 
simultáneos. Los procesos escritores usan la llamada al sistema _send_ para insertar mensajes en la cola, despertando a cualquier lector bloqueado, mientras 
que los procesos lectores utilizan _receive_ para extraer mensajes, bloqueándose con sleep si la cola está vacía y despertándose al recibir nuevos mensajes. 

### Modificaciones al código 
Primeramente, se creó la estructura `message` y la cola `message_queue` en un nuevo archivo llamado _message.h_, con un tamaño y n° de mensajes predefinido.

        #define MAX_MESSAGES 64
        #define MSG_SIZE 128
        typedef struct message {
            int sender_pid;      
            char content[MSG_SIZE]; 
        } message;
        
        typedef struct message_queue {
            struct spinlock lock;            
            message messages[MAX_MESSAGES];   
            int head;                         
            int tail;                             
            int count;                        
        } message_queue;

También, se crearon las llamadas a sistemas `uint64 sys_send(void)` y `uint64 sys_receive(void)`, las cuales se encargan de el envío y recibimiento de mensajes de un proceso a otro, usando un **spinlock** para evitar la modificación de la cola.

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

Estas se definieron, junto con la inicialización de la cola de mensajes, en un archivo llamado `ipc.c`. Además, se actualizaron los archivos _syscall.h, syscall.c, defs.h, usys.pl y user.h_ para que funcionaran
correctamente como llamadas a sistema.  

Finalmente, se creó una prueba llamada `prueba_t5.c`, donde se aplican las funciones y estructura de la cola
de mensajes con un `fork()`: el lector envía un mensaje, y si la cola está llena, recibe un error. Luego el kernel almacena el mensaje en la cola y despierta lectores bloqueados.
Si hay mensajes, se reciben en orden. Y si no hay mensajes, se bloquea hasta que el escritor de mensajes envíe uno.
 Los mensajes se definen como _"Mensaje n-ésimo"_ para asegurar que se envian en el orden correcto, y el programa falla si se tratan de enviar más mensajes de los definidos en `MAX_MESSAGES`.

### Dificultades encontradas

Una de las mayores dificultades fue encontrar el lugar donde se crearían la estructura de mensajes y su cola. Para evitar perturbar la estructura de xv6, se crearon estos nuevos archivos **message.h** e **ipc.c**.  
Además, surgieron problemas con la definición y uso de funciones auxiliares como _argaddr_ y _argstr_, ya que su implementación no coincidía con las declaraciones en algunos casos, lo que generaba errores de tipo, por lo que se debió cambiar su estructura (de void a int que retornara 0).  
Por último, se debieron agregar **guards** (#ifndef, #define, y #endif) en archivos de tipo cabecera, porque, con la agregación de estos nuevos archivos, algunas declaraciones como `#include "defs.h"` se repetían en la compilación. Esto ayudó a evitar errores de este tipo de manera simple.
