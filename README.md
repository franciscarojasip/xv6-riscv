## Informe Tarea 4


### Funcinamiento y lógica
En esta tarea se utiliza una nueva característica de los inodos, llamada `permissions` para modificar los permisos de escritura y lectura de un archivo.

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

En el archivo `sysproc.c`, se definió la nueva llamada al sistema `uint sys_chmod(void)`, donde se busca el inodo correspondiente al archivo, y se 
cambia su característica `permissions` según el número que se haya dado (a menos que el permiso sea inmutable, en este caso
se rechaza el cambio). Junto con la creación de la nueva llamada al sistema, se modificaron todos los archivos correspondientes, para definir esta llamada, como 
`syscall.h` y `syscall.c`.

Finalmente, se creó el archivo de prueba llamado `prueba_t4.c`, donde se asegura la correcta modificación y ejecución de estos permisos.
En ella, se crea un archivo llamado _testfile.txt_, el cual va modificando sus permisos usando la función `chmod`. Se escribe en el archivo un texto de prueba, y se cambian sus permisos a solo lectura. 
Al intentar escribir en el archivo teniendo este permisos solo de lectura, debe aparecer un error indicando que la función cambió los permisos correctamente y no se puede escribir en él. Por último, se cambian los permisos
a inmutable(5), y al intentar cambiar estos permisos, también se devuelve un error.

### Dificultades encontradas
Una gran dificultad fue encontrar la diferencia entre la estructura _dinode_ e _inode_. En el inicio, definí la nueva característica
para ambas estructuras, lo cual trajo problemas con el tamaño de _dinode_, y, por lo tanto, con _BSIZE_. 
Al aprender que _dinode_ es una estructura persistente en el disco, e _inode_ es una copia de esta que aparece en la memoria principal, decidí que solo era necesario
definir el campo `permissions` y realizar las modificaciones correspondientes solo en la estructura de _inode_, evitando asi también problemas de tamaño definido en disco.
