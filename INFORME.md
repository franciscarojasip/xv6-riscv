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
            if (grado == 0) {
                printf("Grado %d: Soy el proceso original, mi PID es %d\n", grado, getpid());
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
    }

    return 0;
}
```

### Dificultades encontradas y cómo se resolvieron
Tuve muchos problemas en la ejecución correcta del código ***yosoytupadre.c***, ya que, al llamar múltiples procesos y sus PID, la respuesta era desordenada, por ejemplo, de la siguiente manera:
> $ yosoytupadre <br>
>SoSyo yu nu np prroocceessoo,,  mmii  PPIDI De ses  34 <br>
>
>EEll  PPIDI deD  mdie  mpir opcreoscoe spoad praed rees  e3s <br>
 >G2r <br>
>adGor a0:d Soo y el0 p: rSoocye seol o rpirgoicneaslo,  omrii gPiInDal ,e sm i4 <br>
 >GPrIaDdo  1es:  A3n <br>
>ceGsrtardoo  con1 :P IADn c3e <br>
>sGrtardoo  c2o:n  APnIcDe s2t <br>
>rGor acdoon 2 :P IADn c2 <br>
>estro con PID 1 <br>

Esto ocurría debido a que, al hacer un fork(), los procesos padre e hijo siguen funcionando de manera paralela, por lo que estaban imprimiendo los resultados de su compilación al mismo tiempo. Para esto, se separan los casos de PID padre e hijo, con un _if_. Lo que se imprime finalmente se enfoca exclusivamente en los resultados desde el proceso hijo, con un _wait(0)_, para que la ejecución del proceso padre no se muestre y no entorpezca la del hijo. <br>
También hubo errores menores de tipografía a la hora de escribir código, como faltas de ; y paréntesis, ya que no estaba acostumbrada a la redacción en C. Estos tuve que resolverlos de a poco, cuando se me presentaban en la ejecución.












