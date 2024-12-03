#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    if (fork() == 0) {
        // Proceso escritor
        for (int i = 0; i < 5; i++) {
            char msg[128];
            snprintf(msg, sizeof(msg), "Mensaje %d", i);
            if (send(getpid(), msg) < 0) {
                printf("Error enviando mensaje %d\n", i);
            }
            sleep(10);  // Simula actividad
        }
        exit(0);
    } else {
        // Proceso lector
        for (int i = 0; i < 5; i++) {
            message msg;
            if (receive(&msg) == 0) {
                printf("Recibido: %s (de PID %d)\n", msg.content, msg.sender_pid);
            } else {
                printf("Error recibiendo mensaje\n");
            }
        }
        wait(0);
    }
    exit(0);
}
