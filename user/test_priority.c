#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int num_processes = 20;
    int i;

    // Crear los 20 procesos hijos
    for (i = 0; i < num_processes; i++) {
        if (fork() == 0) {
            // Código del hijo
            printf("Ejecutando proceso %d\n", getpid());
            sleep(5);  // Cada proceso duerme 5 segundos
            exit(0);   // El hijo termina
        }
        wait(0);  // El padre espera que el hijo termine antes de crear el siguiente
    }

    exit(0);  // El padre también termina
}