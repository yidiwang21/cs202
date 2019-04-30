#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int count = 0;

int
sys_fork(void)
{
  count++;
  return fork();
}

int
sys_exit(void)
{
  count++;
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  count++;
  return wait();
}

int
sys_kill(void)
{
  count++;
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  count++;
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  count++;
  
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  count++;
  
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  count++;
  
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// cs202
int sys_assigntickets(void) {
  count++;
  int num;
  argint(0, &num);
  assigntickets(num);
  return 0;
}

int sys_setsumtickets(void) {
  count++;
  cli();
  int param;
  argint(0, &param);
  setsumtickets(param);
  return 0;
}

int sys_info(void) {
  count++;
  int param;
  argint(0, &param);
  return info(param);
}