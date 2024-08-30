#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"

int main(void) {
    printf("El ID del proceso padre es: %d\n", getppid());
    exit(0);
}
