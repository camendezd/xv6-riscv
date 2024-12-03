Tarea 5

## Crear estructura de datos para mensajes 

para esto definimos la estructura message (linea 31 proc.h)
// Estructura para mensajes (paso 1).
typedef struct message {
    int sender_pid;
    char content[128];
} message;

## Implementar cola de mensajes global

definimos la estructura de cola en proc c:

LINEA 9
// Cola de mensajes global (paso 2)
#define MSG_QUEUE_SIZE 64

typedef struct msg_queue {
    message messages[MSG_QUEUE_SIZE];
    int head;      // Índice de la cabeza
    int tail;      // Índice de la cola
    int size;      // Cantidad de mensajes actuales
    struct spinlock lock;
} msg_queue;

msg_queue mqueue;  // Cola de mensajes global

LINEA 42
// Inicializacion de cola de mensajes
void init_mqueue(void) {
    initlock(&mqueue.lock, "msg_queue");  // spinlock
    mqueue.head = 0;  
    mqueue.tail = 0;  
    mqueue.size = 0;  
}

LINEA 76
  // Inicializar la cola de mensajes
  init_mqueue();

## Implementar sys_send

creamos la llamada al sistema en sysproc.c (linea 95)

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

y declaramos int receive_message(char *content); para evitar errores de declaraciones implicitas

como tambien debemos incluir #include "user.h" para que argptr funcione correctamente

## E implementamos sys_receive

Dentro del mismo archivo sysproc.c (linea 115)

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

## Agregamos la declaracion del prototipo

en codigo defs.h (linea 12)
// Declaración del prototipo de send_message
int send_message(int dest_pid, char *content);

y del prototipo de receive
int receive_message(char *content);

## Volvemos a declarar y agregar la llamada

en codigo syscall.c:

extern uint64 sys_send(void);

[SYS_send]    sys_send,

y en codigo syscall.h:

#define SYS_send    22

## Sincronizar spinlock

en el codigo implementado de sys_send y sys_receive ya se utiliza spinlock para garantizar la sincronizacion

## Por ultimo creamos el programa de prueba ipc_test.c

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

y terminamos añadiendolo a uprogs del makefile.

## Solucion de errores 

### Definición de la estructura msg_queue y MSG_QUEUE_SIZE

por lo que se debia agregar al final de proc.h 

// Definir el tamaño máximo de la cola de mensajes
#define MSG_QUEUE_SIZE 64  

// Estructura para un mensaje
typedef struct message {
    int sender_pid;        
    char content[128];     
} message;

// Estructura para la cola de mensajes
typedef struct msg_queue {
    message messages[MSG_QUEUE_SIZE];  
    int head;
    int tail;
    int size;
    struct spinlock lock;
} msg_queue;

extern msg_queue mqueue;  // Declaración externa de la cola de mensajes

### Inicialización de la cola de mensajes en proc.c

incluir en cabecera de proc.h
#include "proc.h"

definir la funcion 
// Inicializa la cola de mensajes
void init_mqueue(void) {
    initlock(&mqueue.lock, "msg_queue");
    mqueue.head = 0;
    mqueue.tail = 0;
    mqueue.size = 0;
}

y declarar la cola de mensajes globalmente
msg_queue mqueue;  

