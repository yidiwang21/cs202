#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"
#include "semaphore.h"

#define NUMTHREADS 20

lock_t lock;
int thrower;

void player(void *arg_ptr);

int main(int argc, char *argv[]) {
  int i;

  lock_init(&lock);

  for (i = 0; i < NUMTHREADS; i++) {
    thread_create((void*)player, (void*)&i);
    sleep(10);
  }
  while(wait()>=0) ;
  exit();
}

void player(void *arg_ptr) {
  int i, self;
  int *num = (int*) arg_ptr;
  self = *num;

  for (i = 0; i < 10; i++) {
    if (thrower != self) {
      lock_acquire(&lock);
      printf(1, "%d caught frisbee from %d\n", self, thrower);
      thrower = self;
      printf(1, "  throwing frisbee\n", self);
      sleep(20);
      lock_release(&lock);
    }
    sleep(20);
  }
  exit();
}