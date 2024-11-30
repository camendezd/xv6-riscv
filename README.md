Tarea 4 

## Primero agregamos un campo de permisos en la estructura del inode en kernel/fs.h
uint permissions;

## Agregamos ahora la inicializacion del campo permisions en fs.c
dip->permissions = 3;

## Modificamos las operaciones de los archivos en sysfile.c, para que las funciones de apertura, lectura y escritura respeten los permisos:

// Verificar si el archivo es inmutable
if(ip->permissions == 5 && ((omode & O_WRONLY) || (omode & O_RDWR))){
  iunlockput(ip);
  end_op();
  return -1; // No se puede abrir un archivo inmutable en modo escritura
}

// Verificar permisos de escritura
if(((omode & O_WRONLY) || (omode & O_RDWR)) && !(ip->permissions & 2)){
  iunlockput(ip);
  end_op();
  return -1; // No tiene permiso de escritura
}

// Verificar permisos de lectura
if(((omode & O_RDONLY) || (omode & O_RDWR)) && !(ip->permissions & 1)){
  iunlockput(ip);
  end_op();
  return -1; // No tiene permiso de lectura
}

## Definimos la llamada al sistema en syscall.h

#define SYS_chmod 25

## Añadimos a la lista de syscall en syscall.c

[SYS_chmod] sys_chmod,
extern uint64 sys_chmod(void);

## Ahora implementamos la funcion syschmod en sysfile.c

uint64
sys_chmod(void) {
  char path[MAXPATH];
  int mode;
  struct inode *ip;

  // Obtener argumentos: ruta y modo
  if (argstr(0, path, MAXPATH) < 0 || argint(1, &mode) < 0)
    return -1;

  begin_op();
  if ((ip = namei(path)) == 0) {
    end_op();
    return -1; // Archivo no encontrado
  }

  ilock(ip);
  if (ip->permissions == 5) { // Verifica si es inmutable
    iunlockput(ip);
    end_op();
    return -1; // No se pueden cambiar permisos de archivos inmutables
  }

  ip->permissions = mode; // Cambiar permisos
  iunlockput(ip);
  end_op();
  return 0; // Éxito
}


## Por ultimo agregamos la llamada en usys.pl

entry("chmod");

## Para finalizar, agregamos un archivo de prueba user/permtest.c como tambien test_chmod para verificar las segunda parte de la tarea y agregamos al makefile para que se compile correctamente

Contenido en los archivos descritos y makefile se agrego linea $U/_test_chmod\
