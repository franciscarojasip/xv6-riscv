#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Funciones auxiliares
void safestrcpy(char *s, const char *t, int n);
void safestrcat(char *dest, const char *src, int size);
void itoa(int value, char *str, int base);

int main() {
    int pid = fork();

    if (pid < 0) {
        printf("Error: no se pudo crear el proceso\n");
        exit(1);
    }

    if (pid == 0) {
        // Proceso lector
        char buffer[128];
        while (1) {
            int sender = receive(buffer); // Llama a sys_receive
            if (sender >= 0) {
                printf("Mensaje recibido de PID %d: %s\n", sender, buffer);
            } else {
                printf("Error al recibir mensaje\n");
                break; // Salir del bucle en caso de error
            }
        }
    } else {
        // Proceso escritor
        for (int i = 0; i < 5; i++) {
            char msg[128];
            safestrcpy(msg, "Mensaje ", sizeof(msg)); // Copia "Mensaje "
            char num[10];
            itoa(i, num, 10); // Convierte el número a string
            safestrcat(msg, num, sizeof(msg)); // Concatena el número

            if (send(pid, msg) < 0) { // Llama a sys_send
                printf("Error al enviar el mensaje\n");
            }
            sleep(10); // Esperar un poco entre mensajes
        }
        wait(0); // Esperar al proceso lector
    }
    exit(0);
}

// Implementaciones de funciones auxiliares
void safestrcpy(char *s, const char *t, int n) {
    if (n <= 0)
        return;
    while (--n > 0 && (*s++ = *t++) != 0)
        ;
    *s = 0;
}

void safestrcat(char *dest, const char *src, int size) {
    char *d = dest;
    int len = 0;

    while (*d != '\0' && len < size - 1) {
        d++;
        len++;
    }

    while (*src != '\0' && len < size - 1) {
        *d++ = *src++;
        len++;
    }

    *d = '\0';
}

void itoa(int value, char *str, int base) {
    char *ptr = str;
    char *ptr1 = str;
    char tmp_char;
    int tmp_value;

    if (value == 0) {
        *str++ = '0';
        *str = '\0';
        return;
    }

    while (value != 0) {
        tmp_value = value % base;
        *ptr++ = (tmp_value < 10) ? (tmp_value + '0') : (tmp_value - 10 + 'a');
        value /= base;
    }
    *ptr-- = '\0';

    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
}
