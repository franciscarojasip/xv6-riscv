## Informe Tarea 3

### Funcionamiento de la protección de memoria

***mprotect()*** \
Se reciben los argumentos de la localización de memoria ```addr``` y longitud ```len``` que se quiere proteger. Se calcula el límite de esta protección, denominado ```end```. Luego, se recorre cada página dentro de esta región de memoria. En cada una de ellas, se utiliza la función ```walk``` para obtener el puntero de la entrada de la tabla de páginas (donde se encuentran los permisos de escritura de cada página). Si la entrada no existe, se devuelve -1. Finalmente, se cambian los permisos de esta página con ```*pte = *pte & ~PTE_W;```, haciendo así que la página sea de solo lectura.

***munprotect()*** \
El funcionamiento es similar al de ```mprotect()```, solo que, en vez de instaurar un permiso de solo lectura para cada página, este permiso se revoca y devuelve a una página desprotegida, donde se puede escribir.

### Modificaciones realizadas

La modificación principal se encuentra en el archivo ***vm.c***, donde se definieron las funciones *mprotect()* y *munprotect()*:

        int mprotect(void *addr, int len) {
            uint64 a, end;
            pte_t *pte;
        
            a = (uint64) addr;  // Convertir de void* a uint64
            end = a + len;
        
            for (; a < end; a += PGSIZE) {
                pte = walk(myproc()->pagetable, a, 0);
                if (pte == 0)
                    return -1; // Error si la página no existe
        
                // Deshabilitar escritura
                *pte = *pte & ~PTE_W;
            }
        
            return 0; // Éxito
        }
        
        int munprotect(void *addr, int len) {
            uint64 a, end;
            pte_t *pte;
        
            a = (uint64) addr;  // Convertir de void* a uint64
            end = a + len;
        
            for (; a < end; a += PGSIZE) {
                pte = walk(myproc()->pagetable, a, 0);
                if (pte == 0)
                    return -1; // Error si la página no existe
        
                // Restaurar escritura
                *pte = *pte | PTE_W;
            }
        
            return 0; // Éxito
        }

En el archivo ***sysproc.c***, se usan *mprotect()* y *munprotect()* como llamadas de sistema, y se manejan los posibles errores (dirección o largo inválidos):

        int sys_mprotect(void) {
            uint64 addr;
            int len;
        
            // Llamamos a `argaddr` y `argint` de manera independiente
            argaddr(0, &addr);
            argint(1, &len);
        
            // Si la validación falla, retorna -1
            if (addr == 0 || len <= 0)
                return -1;
        
            // Llamada a `mprotect` con los valores obtenidos
            return mprotect((void*)addr, len);
        }
        
        int sys_munprotect(void) {
            uint64 addr;
            int len;
        
            // Llamamos a `argaddr` y `argint` de manera independiente
            argaddr(0, &addr);
            argint(1, &len);
        
            // Si la validación falla, retorna -1
            if (addr == 0 || len <= 0)
                return -1;
        
            // Llamada a `munprotect` con los valores obtenidos
            return munprotect((void*)addr, len);
        }

Al ser llamadas de sistema, ambas funciones se definen en los archivos ***syscall.c*** y ***syscall.h***, con sus definiciones de funciones en ***user.h*** y ***usys.pl***:

        //en syscall.c:
        extern int sys_mprotect(void);
        extern int sys_munprotect(void);
        {...}
        static int (*syscalls[])(void) = {
            ...
            [SYS_mprotect]  sys_mprotect,
            [SYS_munprotect] sys_munprotect,
        };

        //en user.h:
        int mprotect(void *addr, int len);
        int munprotect(void *addr, int len);

        //en usys.pl:
        entry("mprotect");
        entry("munprotect");

Además, se creó una función de prueba llamada ***prueba_t3.c*** en la carpeta ```/user```, para probar estas llamadas, en donde se protege una alocación de memoria, y se puede elegir entre desbloquear el modo lectura, o probar que el espacio está protegido:
        #include "kernel/types.h"
        #include "kernel/stat.h"
        #include "user/user.h"
        
        int main() {
            int len = 1;  // Número de páginas que queremos proteger
            int pagesize = 4096;  // Tamaño de una página de memoria en xv6
            char *addr;
        
            // Paso 1: Reservar memoria
            addr = sbrk(pagesize * len);
            if (addr == (char *)-1) {
                printf("Error al reservar memoria.\n");
                exit(1);
            }
        
            // Paso 2: Marcar la memoria como solo lectura con mprotect
            if (mprotect(addr, len) < 0) {
                printf("Error en mprotect.\n");
                exit(1);
            }
            printf("Memoria marcada como solo lectura con mprotect.\n");
        
            // Paso 3: Intentar escribir en la memoria protegida (debería fallar)
            printf("Intentando escribir en la memoria protegida...\n");
            addr[0] = 'X';  // Esto debería causar un error de protección de memoria
            printf("Error: escritura en memoria protegida fue exitosa, no se esperaba.\n");
        
            // Paso 4: Quitar la protección de solo lectura con munprotect
            if (munprotect(addr, len) < 0) {
                printf("Error en munprotect.\n");
                exit(1);
            }
            printf("Protección de solo lectura eliminada con munprotect.\n");
        
            // Paso 5: Intentar escribir en la memoria nuevamente (debería funcionar)
            printf("Intentando escribir en la memoria sin protección...\n");
            addr[0] = 'Y';
            printf("Escritura exitosa en memoria sin protección: %c\n", addr[0]);
        
            // Liberar memoria
            sbrk(-pagesize * len);
        
            exit(0);
        }

Finalmente, se modificó la función *usertrap()*, en el archivo ***trap.c***, para tener más información acerca del error, como el PID al que le corresponde este error, para asi asegurarse que es por un intento de escritura en un archivo protegido, y no por otra cosa.

### Problemas encontrados y cómo se solucionaron

El problema principal se tuvo haciendo la función de prueba, ya que antes no se tenía la opción de desbloquear, sino que se intentaba escribir en la memoria aun cuando está estaba protegida, y después se utilizaba la función para desbloquear. Esto provocó que existiera un error al intentar escribir en la reserva, pero el código no se seguía ejecutando. Es por esto que se añadió la opción de desbloquear o de intentar escribir, para asi probar que ambas funcionen se ejecutan correctamente.
