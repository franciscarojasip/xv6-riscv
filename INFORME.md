<<<<<<< HEAD
## Informe Tarea 2

Este informe detalla el procedimiento para realizar un programador o scheduler basado en prioridades.

### Funcionamiento y lógica del sistema de prioridades
El sistema recorre todos los procesos que se van asignando.
Después de localizar el proceso n-ésimo, revisa si este tiene la prioridad más alta de los que han pasado hasta ahora (la cual se inicializa como 0). Si es así, se corre el proceso. Al terminarlo, se transforma el proceso en ZOMBIE (para que el programador no lo vuelva a tomar), y se aumenta la prioridad de todos los procesos restantes usando  `p->priority += p->boost`. A la vez, si alguna prioridad es mayor o igual a 9, o menor o igual a 0, los boosts se designan como -1 y 1, respectivamente.
Se considera que, mientras más alto sea el número, mayor prioridad tiene.
### Explicación de las modificaciones realizadas
Primeramente, se debió modificar el archivo `proc.h`, para que la estructura de cada proceso incluyera el campo ***priority*** y ***boost***.

        struct proc {  
         ...
          int priority;  
          int boost;  
        ...
=======
## Informe Tarea 1

### Funcionamiento llamadas
* ***getppid():*** Esta llamada encuentra el PID del proceso padre del actual. Para esto, se hace un fork al inicio, y asi se asegura siempre que exista un proceso hijo. Luego, se define este proceso como el proceso forkeado actualmente. Desde allí, se realiza un puntero hacia el padre del proceso (_proceso->parent->pid_), y se encuentra el PID de este.
* ***getancestor():*** Esta llamada encuentra los ancestros correspondientes del proceso actual. Se usa el mismo proceso forkeado anteriormente en ***getppid()***, para asegurar un hijo y tener más de un ancestro, después de ejecutar el código principal un par de veces. Luego, se definen las opciones de grado (0, 1 y 2), para asi definir un puntero al PID del padre y del abuelo, como se hizo en la llamada anterior. Si el ancestro va más allá del grado 2, se responde que no es posible generarlo. <br>

Ambas llamadas se ejecutan en el archivo ***yosoytupadre.c***. Primero se muestra el funcionamiento de _getppid()_, mostrando el PID del hijo y del padre. Luego, en _getancestor()_ se ejecuta un ciclo _for_ para poder mostrar los grados 0, 1 y 2 del proceso actual. 

### Modificaciones realizadas
Se adjunto el código mencionado en los archivos siguientes:
* ***syscall.h:***
```c
#define SYS_getppid 22
#define SYS_getancestor 23
 ```
* ***syscall.c:***
 ```c
extern uint64 sys_getppid(void);
extern uint64 sys_getancestor(void);
 
 [SYS_getppid] sys_getppid,
[SYS_getancestor] sys_getancestor,
 ```
* ***user.h:***
 ```c
int getppid(void); 
int getancestor(int);
 ```
* ***usys.S:***
 ```c
.global getppid
getppid:
    li a7, SYS_getppid
    ecall
    ret

.global getancestor
getancestor:
    li a7, SYS_getancestor
    ecall
    ret
 ```
* ***sysproc.c:*** <br>

Para definir getppid():
 ```c
 uint64
sys_getppid(void)
{
  return myproc()->parent->pid;
}
```
Para definir getancestor():
```c
uint64 sys_getancestor(void) {
    int grado;
    argint(0, &grado);

    if (myproc()->parent == 0) {
        return -1;
    }
    if (grado == 1) {
        return myproc()->parent->pid;
    }
    if (grado == 2) {
        return myproc()->parent->parent->pid;
    } else {
        printf("No es posible generar este ancestro");
        return -1;
    }
}
```
Finalmente, se creó el archivo llamado ***yosoytupadre.c*** en la carpeta _user_:
```c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int pid = fork();

    if (pid == 0) {
        //getppid
        printf("Soy un proceso hijo, mi PID es %d\n", getpid());
        printf("El PID de mi proceso padre es %d\n", getppid());

        //getancestor
        int grado;
        for (grado = 0; grado <= 2; grado++) {
            if (grado == 0) {<br>
                printf("Grado %d: Soy el proceso original, mi PID es %d\n", grado, getpid());
            } else {
                if (getancestor(grado) == -1) {
                    printf("Grado %d: No se encontró ancestro\n", grado);
                } else {
                    printf("Grado %d: Ancestro con PID %d\n", grado, getancestor(grado));
                }
            }
>>>>>>> 46ad35147 (Update INFORME.md)
        }
Después, se debió modificar el archivo `proc.c`. Tanto en la funcion ***allocproc()***, para que les destine un _priority = 0_ y _boost = 1_ a cada proceso que encuentre:
        static struct proc*
        allocproc(void)
        {
        ...
        found:
          p->pid = allocpid();
          p->state = USED;
          p->priority = 0;
          p->boost = 1;
        ...
        }
Como en la parte del ***scheduler()***, 
        PONER CÓDIGO ACA

También, se creó un archivo llamado ***prueba_t2.c***, donde se ve este programador de prioridades en acción:
        PONER CODIGO ACA

### Dificultades y cómo se resolvieron

La primera dificultad se encontró cuando, al comenzar la ejecución del sistema (make qemu), las salidas de los printf puestos para depurar el código se veían de manera desordenada, como por ejemplo: 

        xv6 kernel is booting

        hart 1 starting  
        hart 2 starting  
        Process 1 has priority 0 and boost 1  
        Candidate process: pid 1, priority 1  
        Process 1 has priority 1 and boost 1  
        Candidate process: pid 1, priority 2  
        Process 1 has priority 2 and boost 1  
        Candidate process: pid 1, priority 3  
        Switching to process: pid 1, priority 3  
        Switching to process: pid 1, priority 3  
        Switching to process: pid 1, priority 3  
        Unknown trap: scause=0x8 sepc=0x3fffffdf90 stval=0x800053dc  
        Unknown trap: scause=0xc sepc=0x0 stval=0x0  
        panic: panic: UnUnknown trapknown trap: scau: scause=0xc sepc=0x200000100 stval=0x200se=0x3ff000100  
        panic: fffdc11 sepc=0x0 stkval=0x8000fernel7a0  
        Unknown trap: trap  
        s  

Aqui se puede ver que se ejecuta el mismo código más de una vez, imprimiendo varios errores simultáneos. Esto ocurría debido a que el SO estaba ejecutándose con 3 CPUs. Por esto, se debío minimizar la cantidad de CPUs en el Makefile, a 1.









