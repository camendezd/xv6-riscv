#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "user.h"

int receive_message(char *content);

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64 
sys_send(void) {
    int dest_pid;
    char content[128];  // Definir un buffer para el contenido del mensaje

    // Obtener los argumentos: dest_pid (int) y content (char*)
    if (argint(0, &dest_pid) < 0 || argstr(1, content, sizeof(content)) < 0)
        return -1;

    // Asegúrate de que la cola de mensajes no esté llena antes de enviar
    acquire(&mqueue.lock);
    if (mqueue.size >= MSG_QUEUE_SIZE) {
        release(&mqueue.lock);
        return -1;  // La cola está llena
    }

    // Insertar el mensaje en la cola
    mqueue.messages[mqueue.tail].sender_pid = myproc()->pid;
    strncpy(mqueue.messages[mqueue.tail].content, content, sizeof(content));
    mqueue.tail = (mqueue.tail + 1) % MSG_QUEUE_SIZE;  // Actualizar la cola circular
    mqueue.size++;
    release(&mqueue.lock);
    
    return 0;  // Enviado exitosamente
}

uint64 
sys_receive(void) {
    message *msg;
    
    // Obtener el argumento: un puntero al buffer de mensaje
    if (argptr(0, (void *)&msg, sizeof(*msg)) < 0)
        return -1;

    // Asegúrate de que la cola de mensajes no esté vacía
    acquire(&mqueue.lock);
    if (mqueue.size == 0) {
        release(&mqueue.lock);
        return -1;  // No hay mensajes
    }

    // Recibir el mensaje de la cola
    *msg = mqueue.messages[mqueue.head];
    mqueue.head = (mqueue.head + 1) % MSG_QUEUE_SIZE;  // Actualizar la cola circular
    mqueue.size--;
    release(&mqueue.lock);

    return 0;  // Mensaje recibido exitosamente
}

