#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    // Crear procesos aquí usando fork(), por ejemplo:
    for (int i = 0; i < 20; i++) {
        if (fork() == 0) {
            // Código del proceso hijo
            //printf("Soy el proceso hijo con PID %d\n", getpid());
            sleep(5); // Simular trabajo
            exit(0);
        }
    }

    // Esperar a que todos los hijos terminen
    for (int i = 0; i < 20; i++) {
        wait(0);
    }

    exit(0);
}
