#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  // Reservar una página de memoria y hacer el cast a int*
  int *mem = (int*) sbrk(4096);
  if (mem == (void*)-1) {
    printf("Error al reservar memoria.\n");
    exit(1);
  }

  // Proteger la memoria como solo lectura
  if (mprotect(mem, 1) < 0) {
    printf("Error al marcar la memoria como solo lectura.\n");
    exit(1);
  }
  printf("Memoria marcada como solo lectura con mprotect.\n");

  // Intentar leer la memoria protegida (debería funcionar)
  int valor = *mem;
  printf("Lectura de la memoria protegida exitosa: %d\n", valor);

  // Pedir al usuario si desea escribir en la memoria protegida
  printf("¿Desea intentar escribir en la memoria protegida? (Presione 1 para intentar, Enter para continuar): ");

  // Leer un solo carácter de la entrada estándar
  char opcion;
  if (read(0, &opcion, 1) < 0) {
    printf("Error al leer la entrada.\n");
    exit(1);
  }

  if (opcion == '1') {
    // Intentar escribir en la memoria protegida (esto debería causar una excepción)
    printf("Intentando escribir en la memoria protegida...\n");
    *mem = 123;  // Esperamos una excepción aquí

    // Código que no se ejecutará si ocurre una excepción:
    printf("Escritura en memoria protegida exitosa (esto no debería suceder).\n");
  }

  // Eliminar la protección de solo lectura
  if (munprotect(mem, 1) < 0) {
    printf("Error al eliminar la protección de la memoria.\n");
    exit(1);
  }
  printf("Protección eliminada con munprotect.\n");

  // Intentar escribir en la memoria nuevamente, ahora sin protección
  *mem = 456;  // Esto debería funcionar
  printf("Escritura en la memoria sin protección exitosa: %d\n", *mem);

  exit(0);
}
