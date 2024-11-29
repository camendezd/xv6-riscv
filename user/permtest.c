#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main() {
  int fd = open("testfile", O_CREATE | O_RDWR);
  write(fd, "hello", 5);
  close(fd);

  chmod("testfile", 1); // Cambiar a solo lectura
  fd = open("testfile", O_WRONLY); // Debe fallar
  if (fd >= 0) {
    printf("Error: Permiso de escritura no respetado\n");
    close(fd);
  }

  chmod("testfile", 3); // Cambiar a lectura/escritura
  fd = open("testfile", O_RDWR); // Debe funcionar
  if (fd >= 0) {
    write(fd, "world", 5);
    close(fd);
  }
  exit(0);
}
