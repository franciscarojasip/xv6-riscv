#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "file.h"
#include "fs.h"
#include "sleeplock.h"

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

uint 
sys_chmod(void) 
{
    char archivo[MAXPATH]; // Buffer para el nombre del archivo
    int permissions;
    struct inode *ip;

    if (argstr(0, archivo, MAXPATH) < 0) {
        return -1; // Error al obtener el nombre del archivo
    }

    argint(1, &permissions);

    if (archivo[0] == '\0' || permissions < 0) {
        return -1; // Validación adicional para evitar errores
    }

    // Comenzar operación en el sistema de archivos
    begin_op();

    // Buscar el inode del archivo por nombre
    if ((ip = namei(archivo)) == 0) { // Busca el archivo por nombre
        end_op();
        return -1; // Archivo no encontrado
    }

    // Bloquear el inode
    ilock(ip);

    // Verificar si el archivo es inmutable (por ejemplo, si tiene un permi>    if (ip->permissions == 5) {
     if (ip->permissions == 5) {
	iunlockput(ip);
        end_op();
        return -1; // No se puede cambiar permisos de un archivo inmutable
    }

    // Cambiar los permisos
    ip->permissions = permissions;

    // Actualizar el inode en el disco
    iupdate(ip);

    // Desbloquear y liberar el inode
    iunlockput(ip);

    // Finalizar operación en el sistema de archivos
    end_op();

    return 0; // Éxito
}
