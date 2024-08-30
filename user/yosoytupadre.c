#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int getancestor(int grado);  

int main() {
//getppid
  int pid = fork();
  if (pid == 0) {
    printf("Soy un proceso hijo, mi PID es %d\n", getpid());
    printf("El PID de mi proceso padre es %d\n", getppid());
// getancestor
     int grado;
     for (grado = 0; grado <= 2; grado++) {
       if (grado == 0) {
         printf("Soy el proceso original, mi PID es %d\n", getpid());
     } else {
       if (getancestor(grado) == -1) {
         printf("Grado %d: No se encontró ancestro\n", grado);
     } else {
         printf("Grado %d: Ancestro con PID %d\n", grado, getancestor(grado));
}
}
}

 } else {
    wait(0);  
    printf(" Soy un proceso padre, mi PID es %d\n", getpid());
    }

    return 0;
}

