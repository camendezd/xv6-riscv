// test_chmod.c

#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
    char *filename = "testfile";
    
    // Crear un archivo con permisos de lectura/escritura
    int fd = open(filename, O_CREATE | O_RDWR);
    if (fd < 0) {
        printf(1, "Error al crear el archivo\n");
        exit();
    }

    // Escribir algo en el archivo
    write(fd, "Hola Mundo\n", 11);
    close(fd);

    // Cambiar los permisos a solo lectura (chmod)
    chmod(filename, 1);
    printf(1, "Permisos cambiados a solo lectura\n");

    // Intentar escribir en el archivo (debería fallar)
    fd = open(filename, O_RDWR);
    if (fd >= 0) {
        printf(1, "ERROR: Debería fallar al abrir el archivo en modo lectura/escritura\n");
        close(fd);
        exit();
    } else {
        printf(1, "Es correcto: No se puede abrir el archivo en modo lectura/escritura\n");
    }

    // Cambiar los permisos a lectura/escritura
    chmod(filename, 3);
    printf(1, "Permisos cambiados a lectura/escritura\n");

    // Intentar escribir nuevamente
    fd = open(filename, O_RDWR);
    if (fd < 0) {
        printf(1, "Error al abrir el archivo en modo lectura/escritura\n");
        exit();
    }
    write(fd, "Adiós Mundo\n", 12);
    close(fd);

    printf(1, "Prueba de chmod completada\n");
    exit();
}
