
## Informe Tarea 2

Este informe detalla el procedimiento para realizar un programador o scheduler basado en prioridades.

### Funcionamiento y lógica del sistema de prioridades
El sistema recorre todos los procesos que se van asignando.
Después de localizar el proceso n-ésimo, revisa si este tiene la prioridad más alta de los que han pasado hasta ahora (la cual se inicializa como 0). Si es así, se corre el proceso. Al terminarlo, se transforma la variable _executed_ a 1, para que se sepa que ese proceso ya fue procesado, y se aumenta la prioridad de todos los procesos restantes usando  `p->priority += p->boost`. A la vez, si alguna prioridad es mayor o igual a 9, o menor o igual a 0, los boosts se designan como -1 y 1, respectivamente.
Se considera que, mientras más alto sea el número, mayor prioridad tiene.
### Explicación de las modificaciones realizadas
Primeramente, se debió modificar el archivo `proc.h`, para que la estructura de cada proceso incluyera el campo ***priority*** y ***boost***.

        struct proc {  
         ...
          int priority;  
          int boost;  
        ...

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
  
Como en la parte del ***scheduler()***. Aqui, se recorren todos los procesos en cola, y se buscan aquellos que estén en estado RUNNABLE y su variables _executed_ sea igual a 0. Si se encuentra una mayor prioridad en el proceso p, este se ejecuta y se marca como tal. Después, todos los procesos restantes que no hayan sido ejecutados son aumentados en su boost.

        void scheduler(void) {
        struct proc *p;
        struct cpu *c = mycpu();
        c->proc = 0;
        struct proc *highest_priority = 0;

        for (;;) {
         intr_on();

         for (p = proc; p < &proc[NPROC]; p++) {
            if (p->state == 3 && p->executed == 0) { // RUNNABLE y no ejecutado
                // Buscar el proceso con mayor prioridad
                if (highest_priority == 0 || p->priority > highest_priority->priority) {
                    highest_priority = p;
                    acquire(&p->lock);
                   c->proc = p; printf("Ejecutando proceso %d con prioridad %d\n", p->pid, p->priority);
                    swtch(&c->context, &p->context);
                    p->executed = 1;
            // Marcar el proceso p (que es highest_priority en este punto) como ejecutado
                   release(&p->lock);
                }
                else {
            // Si no hay procesos ejecutables, poner la CPU en espera
            intr_on();
            asm volatile("wfi");
                }
            }

        }

        // Después de cada iteración, ajustar las prioridades de los procesos RUNNABLE
        for (p = proc; p < &proc[NPROC]; p++) {
            if (p->state == RUNNABLE && p->executed == 0) { // Solo ajustar si no ha sido ejecutado
                // Ajustar la prioridad de los procesos que no fueron seleccionados
                p->priority += p->boost;

                // Invertir el boost si la prioridad llega a los límites (0 o 9)
                if (p->priority >= 9) {
                    p->boost = -1;
                } else if (p->priority <= 0) {
                    p->boost = 1;
                }
            }
        }
    }

También, se creó un archivo llamado ***prueba_t2.c***, donde se ve este programador de prioridades en acción:
        #include "kernel/types.h"
        #include "kernel/stat.h"
        #include "user/user.h"
        
        int main(int argc, char *argv[]) {
            // Crear procesos aquí usando fork(), por ejemplo:
            for (int i = 0; i < 10; i++) {
                if (fork() == 0) {
                    // Código del proceso hijo
                    //printf("Soy el proceso hijo con PID %d\n", getpid());
                    sleep(5); // Simular trabajo
                    exit(0);
                }
            }
        
            // Esperar a que todos los hijos terminen
            for (int j = 0; j < 10; j++) {
                wait(0);
            }
        
            exit(0);
        }

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










