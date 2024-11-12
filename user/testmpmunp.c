#include "user.h"

int main() {
    char *ptr = sbrk(0);  // Obtener el fin de la memoria del proceso
    sbrk(4096);           // Reservar una página adicional

    if (mprotect(ptr, 1) < 0) {
        printf("Error en mprotect\n");
        exit(1);
    }

    printf("Intentando escribir en una página read-only...\n");
    *ptr = 'A';  // Esto debería causar un fallo de segmentación

    if (munprotect(ptr, 1) < 0) {
        printf("Error en munprotect\n");
        exit(1);
    }

    *ptr = 'B';  // Esto debería funcionar sin problemas
    printf("Escritura exitosa después de munprotect\n");

    exit(0);
}
