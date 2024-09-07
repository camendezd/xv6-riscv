Para crear esta nueva funcion en xv6 llamada getppid fue necesario primero agregar al uprogs del makefile la linea de 

$U/_getppid\ 

(aqui ya surgieron varios problemas con el nombre en algunos momentos del trabajo), 
lo siguiente definir 

#define SYS_getppid  22  
en syscall.h, despues agregamos en syscall,c 

extern uint64 sys_getppid(void); 
[SYS_getppid]   sys_getppid,

agregamos tambien en el archivo user.h 

int getppid(void);

como tambien en usys.pl

entry("getppid");

luego crear en sysproc.c la funcion de la llamada

uint64
sys_getppid(void)
{
    struct proc *p = myproc();  // Obtiene el proceso actual
    if(p->parent)               // Si tiene un padre, retorna su ID
        return p->parent->pid;
    return -1;                  // Si no tiene padre (proceso huérfano), retorna -1
}

para finalmente crear un archivo en user llamado getppid.c

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void) {
    int ppid = getppid();
    printf("Parent Process ID: %d\n", ppid);
    exit(0);
}

Luego de todo lo anterior procedi a hacer los mismos pasos para el get ancestor pero no logre que funcionara a pesar de mitigar constantemente errores, en un momento llegue a rendirme y borre todas las modificaciones para que solamente quedara con la parte 1 de la tarea (getppid) pero me dio error, asi que tuve que solucionar esos errores de nuevo y logre hacer que sea funcional la llamada.