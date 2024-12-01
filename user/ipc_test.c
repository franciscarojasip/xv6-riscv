#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int pid = fork();

    if (pid < 0) {
        printf("Error: fork falló\n");
        exit(1);
    }

    if (pid == 0) {
        // Proceso lector
        char buffer[128];
        while (1) {
            int sender_pid = receive(buffer);
            if (sender_pid >= 0) {
                printf("Mensaje recibido de %d: %s\n", sender_pid, buffer);
            }
        }
    } else {
        // Proceso escritor
        char message[128];
        for (int i = 0; i < 5; i++) {
            snprintf(message, sizeof(message), "Mensaje %d", i);
            send(pid, message);
            sleep(10); // Pausa entre mensajes
        }
        wait(0);
    }
    exit(0);
}
