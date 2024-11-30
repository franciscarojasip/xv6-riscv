#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"
#include "../kernel/fs.h"
#include "../kernel/fcntl.h"

int main() {
    char *filename = "testfile.txt";
    int fd;

    // Crear un archivo con permisos de lectura y escritura (O_CREATE | O_RDWR)
    fd = open(filename, O_CREATE | O_RDWR);
    if (fd < 0) {
        printf("Error al crear el archivo\n");
        exit(1);
    }

    // Escribir en el archivo para asegurarse de que funciona en modo lectura/escritura
    if (write(fd, "Contenido inicial", 17) != 17) {
        printf("Error al escribir en el archivo\n");
        exit(1);
    }
    close(fd);

    // Cambiar los permisos a solo lectura (chmod(filename, 1))
    if (chmod(filename, 1) < 0) {
        printf("Error al cambiar permisos a solo lectura\n");
        exit(1);
    }

    // Intentar escribir en el archivo con permisos de solo lectura (debe fallar)
    fd = open(filename, O_WRONLY);
    if (fd >= 0) {
        printf("Se debería haber producido un error al intentar escribir en modo solo lectura\n");
        close(fd);
        exit(1);
    }
    printf("Correcto: No se puede escribir en el archivo con permisos de solo lectura\n");

    // Cambiar los permisos a lectura/escritura (chmod(filename, 3))
    if (chmod(filename, 3) < 0) {
        printf("Error al cambiar permisos a lectura/escritura\n");
        exit(1);
    }

    // Escribir nuevamente en el archivo para confirmar que los permisos de escritura han sido restaurados
    fd = open(filename, O_RDWR);
    if (fd < 0) {
        printf("Error al abrir el archivo para escribir\n");
        exit(1);
    }
    if (write(fd, "Escritura final", 15) != 15) {
        printf("Error al escribir en el archivo\n");
        exit(1);
    }
    close(fd);

    // Cambiar los permisos a inmutable (chmod(filename, 5))
    if (chmod(filename, 5) < 0) {
        printf("Error al cambiar permisos a inmutable\n");
        exit(1);
    }

    // Intentar escribir en el archivo con permisos inmutables (debe fallar)
    fd = open(filename, O_WRONLY);
    if (fd >= 0) {
        printf("Se debería haber producido un error al intentar escribir en el archivo inmutable\n");
        close(fd);
        exit(1);
    }
    printf("Correcto: No se puede escribir en el archivo inmutable\n");

    // Intentar cambiar los permisos de vuelta a lectura/escritura (debe fallar)
    if (chmod(filename, 3) >= 0) {
        printf("Se debería haber producido un error al intentar cambiar los permisos de un archivo inmutable\n");
        exit(1);
    }
    printf("Correcto: No se puede cambiar los permisos de un archivo inmutable\n");

    printf("Pruebas listas\n");
    exit(0);
}
