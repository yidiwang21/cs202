# CS202 Lab 2 Report
Yidi Wang </br>
862114701

## Modifications
### clone() System Call
The ```clone(...)``` system call is located in ```proc.c```, and is written based on fork() system call.

#### Address Space
Different from ```fork()``` system call, clone() system call uses parent's address space.
```
np->state = UNUSED;
np->sz = currproc->sz;
np->parent = currproc;
*np->tf = *currproc->tf;
np->pgdir = currproc->pgdir;
```

#### File Descriptor
The thread should use the same file descriptor as parent. This is copied as in ```fork()```.
```
for (i = 0; i < NOFILE; i++)
    if (currproc->ofile[i])
        np->ofile[i] = filedup(currproc->ofile[i]);
np->cwd = idup(currproc->cwd);
safestrcpy(np->name, currproc->name, sizeof(currproc->name));
```

#### User Stack
I created an array ```ustack[]``` to store the passed arguments. Then I used kernel function copyout() to copy ustack to the virtual address in page table pgdir. Register ```$esp``` stores the address of the top of the stack, pointing to ```0xFFFFFFFF```. Then the following addresses should point to the input arguments. This is based on ```exec.c```.
After decide the address of the top of the stack, register ```$eip``` is set to point to the next instruction the system is about to execute, and the base pointer register ```$ebp``` is set to be the same as ```$esp```.
```
uint ustack[2];
ustack[0] = 0xffffffff;  // fake return PC
ustack[1] = (uint)arg;

size = sizeof(ustack);
np->tf->esp = (uint)(stack+PGSIZE - 4); //put esp to right spot on stack
*((uint*)(np->tf->esp)) = (uint)arg; //arg to function
*((uint*)(np->tf->esp) - 4) = 0xFFFFFFFF; //return to nowhere
np->tf->esp =(np->tf->esp) - 4;
if (copyout(np->pgdir, np->tf->esp, ustack, size) < 0) {
        cprintf("Stack copy failed.\n");
        return -1;
}

np->tf->eip = (uint)func; // set instruction pointer
np->tf->eax = 0;          // clear %eax so that fork returns 0 in the child.
np->tf->ebp = currproc->tf->esp;  // set base pointer
```

### Thread Library
The library is located in ```ulib.c```.
#### thread_create() function
I first malloc 2 pages for stack. If the virtual address of start of the stack is not page-aligned, I round it up. Then ```clone()``` system call is called. In this case, only one argument of type "unsigned int" is passed to the function, so the size is assigned to be 8. The new thread starts executing at the address specified by ```start_routine()```.
```
void *thread_create(void*(start_routine)(void*), void *arg) {
  void *stack = malloc(2 * PGSIZE);
  if((uint)stack % PGSIZE)
    stack = stack + (PGSIZE - (uint)stack % PGSIZE);

  int size = 8;
  int tid = clone(start_routine, stack, size, arg);
  if (tid < 0) {
    printf(1, "# Clone failed\n");
    return 0;
  }
}
```

#### Spinlock
First, I created a new data type in ```types.h``` to store the lock.
```
typedef struct {
    uint locked;
}lock_t;
```

Then, the implementation for the spinlock in the thread library is similar to the original kernel spinlock in xv6.
```
void lock_init(lock_t *lock) {
  lock->locked = 0;
}

void lock_acquire(lock_t *lock) {
  while(xchg(&(lock->locked), 1) != 0);
}

void lock_release(lock_t *lock) {
  xchg(&(lock->locked), 0);
}
```

## Test
In the test program, the user-defined number of threads are created to run the same function. The variables pass round ```pass_round``` and pass number ```pass_no``` are kept globally in the program. They modify the values of the global variables in critical region, which is guaranteed by the spinlock implemented in uer library.

### Usage
```
make qemu
labtest [thread number] [pass round]
```
If no input argument is given, the default number of thread is 4 and pass round is 6.

### Test Program
```
#include "user.h"
#include "types.h"

int pass_no = 0;
int pass_round = 0;
int thread_num;
int thrower;

lock_t lock;

void player(void* arg) {
    int tid = *(int*)arg;

    int pass_num = (tid <= pass_round % thread_num) ? (pass_round / thread_num + 1) : (pass_round / thread_num);

    int i;
    for (i = 0; i < pass_num; i++) {
        if (thrower != tid) {
            lock_acquire(&lock);
            pass_no++;
            printf(1, "# Pass number no: %d | ", pass_no);
            printf(1, "Thread %d is passing the token to thread %d\n", thrower, tid);
            thrower = tid;
            lock_release(&lock);
            sleep(20);
        }
        sleep(20);
    }

    exit();
}

int main(int argc, char *argv[]) {
    lock_init(&lock);

    if (argc != 3) {
        thread_num = 4;
        pass_round = 6;
        printf(1, "# Default thread number: 4, pass round: 6\n");
    }else {
        thread_num = atoi(argv[1]);
        pass_round = atoi(argv[2]);
    }

    int i;
    uint arg = 0;
    for (i = 0; i < thread_num; i++) {  // i is tid
        arg = i + 1;
        thread_create((void*)player, (void*)&arg);
        sleep(10);
    }
    sleep(100);

    printf(1, "# Simulation of Frisbee game has finished, 6 rounds were played in total!\n");
    exit();
}
```

### Results
![alt text](https://github.com/yidiwang21/cs202/blob/master/lab2/report/figs/fig1.png?raw=true)

![alt text](https://github.com/yidiwang21/cs202/blob/master/lab2/report/figs/fig2.png?raw=true)