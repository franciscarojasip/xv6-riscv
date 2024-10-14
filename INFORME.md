
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









