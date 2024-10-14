#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int pid;

    // Crear varios procesos para verificar el sistema de prioridad
    for(int i = 0; i < 5; i++) {
        pid = fork();
        if(pid == 0) {
            printf("Proceso hijo %d con PID: %d iniciado\n", i, getpid());
            while(1);  // Bucle infinito para mantener los procesos corriendo
        }
    }

    // Esperar un momento para ver cómo cambian las prioridades
    sleep(100);

    // Finalizar los procesos
    for(int i = 0; i < 5; i++) {
        kill(pid);
    }

    exit(0);
}
