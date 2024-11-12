# Informe 

## Iniciamos agregando las llamadas

Makefile
$U/_testmpmunp\ para testear los cambios con el programa testmpmunp.c

User.h 
int mprotect(void *addr, int len);
int munprotect(void *addr, int len);

Usys.pl
entry("mprotect");
entry("munprotect");

syscall.h
#define SYS_mprotect 23
#define SYS_munprotect 24

syscall.c
[SYS_mprotect]   sys_mprotect,
[SYS_munprotect] sys_munprotect,

## Luego definiremos las funciones de mprotect y munprotect 

vm.c

int mprotect(void *addr, int len) {
    struct proc *p = myproc();
    pte_t *pte;
    for (int i = 0; i < len; i += PGSIZE) {
        pte = walk(p->pagetable, (uint64)(addr + i), 0);
        if (pte == 0)
            return -1; 
        *pte &= ~PTE_W;  
    }
    return 0;
}

esto nos retorna una direccion invalida en caso de como mismo
elimina el bit de escritura para hacerlo read-only

int munprotect(void *addr, int len) {
    struct proc *p = myproc();
    pte_t *pte;
    for (int i = 0; i < len; i += PGSIZE) {
        pte = walk(p->pagetable, (uint64)(addr + i), 0);
        if (pte == 0)
            return -1;
        *pte |= PTE_W;  
    }
    return 0;
}

Al igual que el anterior nos retorna en caso de direccion invalida
como asi activa el bit de escritura para permitir lectura/escritura

int sys_mprotect(void) {
    void *addr;
    int len;
    if (argptr(0, (char **)&addr, sizeof(void *)) < 0 || argint(1, &len) < 0)
        return -1;
    return mprotect(addr, len);
}

int sys_munprotect(void) {
    void *addr;
    int len;
    if (argptr(0, (char **)&addr, sizeof(void *)) < 0 || argint(1, &len) < 0)
        return -1;
    return munprotect(addr, len);
}

## Por ultimo haremos el programa que testee los cambios

testmpmunp.c:

#include "user.h"

int main() {
    char *ptr = sbrk(0);  // Obtener el fin de la memoria del proceso
    sbrk(4096);           // Reservar una página adicional

    if (mprotect(ptr, 1) < 0) {
        printf("Error en mprotect\n");
        exit(1);
    }

    printf("Intentando escribir en una página read-only...\n");
    *ptr = 'A';  

    if (munprotect(ptr, 1) < 0) {
        printf("Error en munprotect\n");
        exit(1);
    }

    *ptr = 'B';  
    printf("Escritura exitosa después de munprotect\n");

    exit(0);
}
