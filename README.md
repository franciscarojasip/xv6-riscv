## Informe Tarea 4


### Funcinamiento y lógica
Se modifican las características de los inodos, los cuales contienen los metadatos de un archivo. Se agrega una nueva característica, llamada _permissions_, en donde se logra definir si el archivo puede leerse y escribirse (3), solo leerse (1) ó solo lectura, sin poder cambiar este estado (5).  
Después, se utiliza esta característica en la llamada al sistema `chmod`, donde se modifican estos permisos, según el nombre de archivo y permiso que se da.  
Finalmente, esta llamada se implementa en un archivo de prueba, para asegurarse que los permisos funcionan correctamente.

### Modificaciones al código 
En el archivo `file.h`, donde se encuentra decalarada la estructura de los inodos, se le agregó este campo llamado `permissions`:

        struct inode {
          uint dev;           // Device number
          uint inum;          // Inode number
        ...
          int permissions;
          uint size;
          uint addrs[NDIRECT+1];
        };  
        
En el archivo `fs.c`, donde se encuentran funciones relacionadas a los inodos, se modificó la función `ialloc(uint dev, short type)`, para que, al identificar un inodo, este incluyera los permisos por defecto
` ip->permissions = 3;`.  

En el archivo `sysfile.c`, se modifica la función `uint64 sys_open(void)`, para que no se puedan abrir los archivos que son de modo lectura en modo escritura, o abrir directamente directorios en modo escritura (ya que no es válido). Si alguna de estas verificaciones no es la correcta, retorna -1.  

En el archivo `sysproc.c`, se definió la nueva llamada al sistema `uint sys_chmod(void)`, donde se busca el inodo correspondiente al archivo, y se 
cambia su característica `permissions` según el número que se haya dado (a menos que el permiso sea inmutable, en este caso
se rechaza el cambio). Junto con la creación de la nueva llamada al sistema, se modificaron todos los archivos correspondientes, para definir esta llamada, como 
`syscall.h` y `syscall.c`.

Finalmente, se creó el archivo de prueba llamado `prueba_t4.c`, donde se asegura la correcta modificación y ejecución de estos permisos.
En ella, se crea un archivo llamado _testfile.txt_, el cual va modificando sus permisos usando la función `chmod`. Se escribe en el archivo un texto de prueba, y se cambian sus permisos a solo lectura. 
Al abrir el archivo nuevamente, con modo escritura `(O_WRONLY)`, debe aparecer un error indicando que la función cambió los permisos correctamente y no se puede escribir en él. Luego, se vuelven a cambiar los permisos de nuevo a escritura y lectura, escribiendo otro texto para confirmalo. Por último, se cambian los permisos
a inmutable(5), y al intentar cambiar estos permisos, también se devuelve un error.

### Dificultades encontradas
Una gran dificultad fue encontrar la diferencia entre la estructura _dinode_ e _inode_. En el inicio, definí la nueva característica
para ambas estructuras, lo cual trajo problemas con el tamaño de _dinode_, y, por lo tanto, con _BSIZE_. 
Al aprender que _dinode_ es una estructura persistente en el disco, e _inode_ es una copia de esta que aparece en la memoria principal, decidí que solo era necesario
definir el campo `permissions` y realizar las modificaciones correspondientes solo en la estructura de _inode_, evitando asi también problemas de tamaño definido en disco.  
Además, hubo una dificultad al intentar subir los cambios hechos de manera local a la rama del repositorio remoto, ya que algunos archivos de `riscv-gnu-toolchain` y `qemu` eran demasiado grandes. Luego de muchos intentos fallidos, se debió hacer un SFTP entre la máquina virtual y el computador, para transferir los archivos y subirlos de manera manual a GitHub.
