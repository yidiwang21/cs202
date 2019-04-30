# CS202 Lab 1 Report
Yidi Wang </br>
862114701

## Part 1
### Main Idea
I added a system definition in proc.c, and accordingly, I changed the other files to make it work.
* The first option is to print the number of processes of any state in the current system. I simply traverse the pcb table to count the total number.
* The second option is to print the number of system call the current process has done so far. I declared a global counter in defs.h which also accomodate the declaration of system calls. I increment the counter in each function inside sysproc.c.
* In xv6, the virtual memory stack grows from bottom to top. There is a field "sz" kept in pcb, which is the size of stack. So I round it up, and divide by the page size, the result should be the number of pages the current process uses.

### Notes on Test File
After compiling, the test file can be simply run by:
```
$ part1 [1, 2, or 3]
```

### Modifications
proc.c:
```
int info(int param) {
  struct proc *p;

  if (param == 1) {
    int cnt = 0;
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++) { 
      cnt++;
    }
    return cnt;
  }else if (param == 2) {
    return count;
  }else if (param == 3) {
    int cnt = 0;
    cnt = PGROUNDUP(myproc()->sz) / PGSIZE;
    return cnt;
  }
  return -1;
}
```

user.h:
```
// system calls
...
int assigntickets(int);     // cs202 part2
int info(int);              // cs202 part1
```

defs.h:
```
//PAGEBREAK: 16
// proc.c
...
void            assigntickets(int); // cs202 part2
int             info(int);          // cs202 part1
```
```
// cs202 part1
// system call counter
extern int count;
```

syscall.h:
```
#define SYS_assigntickets   22
#define SYS_info   23
```

syscall.c:
```
extern int sys_assigntickets(void); // cs202 part2
extern int sys_info(void);          // cs202 part1

static int (*syscalls[])(void) = {
...
[SYS_assigntickets] sys_assigntickets,  // cs202 part2
[SYS_info]    sys_info,                 // cs202 part2
};
```

usys.S:
```
SYSCALL(assigntickets)
SYSCALL(info)
```

sysproc.c:
```
int count = 0;
...
int sys_info(void) {
  count++;
  int param;
  argint(0, &param);
  return info(param);
}
```

### Test Results
![alt text](https://github.com/yidiwang21/cs202/blob/master/lab1/figs/part1_demo.png?raw=true)


## Part 2
### Main Idea
The first thing I changed is in Makefile, in which I changed the number of cpu from 2 to 1. </br>
I added three field in pcb located in proc.h. ```tickets``` keeps the number of tickets that a process is assigned. ```stride``` is first set to be a large number. When scheduler is triggered, ```stride``` equals to total number of tickets divided by the process's tickets. ```pass``` keeps the next time when the process is allowed to run. Each time when a process is scheduled, ```pass``` should be incremented by ```stride```.  </br>
I defined some macros related to tickets in ```param.h```. The fields are initialized in system function ```allocproc()```. </br>
An system call ```assigntickets``` is written for user space program to call to give some tickets to the current process. The number of tickets for the process as well as the ```stride``` is updated n the system call. Because the scheduling ratio of each process should be proportional to the inverse of its stride, I don't need to calculated the total number of tickets all the processes at "ready" or "running" state are holding.
In ```scheduler()```, I traversed the pcb table again to find the process of a minimum ```pass``` value, which means that the process is eligible to run earliest.

### Notes on Test File
After compiling, the test file can be simply run by:
```
$ part2
```
In this test file, I used "fork()" system call twice to launch three processes in total. I did this to track their pids. After the process with the maximum tickets finishes, the process will kill the other two processes. In this way, the final prints are the relative ticks each process has been scheduled. </br>
Additionally, to guarantee the functionality of the program, if this file need to be changed, the "prog1" must be assigned most tickets, because I keep track of their process ids in this process.

### Modifications
(The other changes has been illustrated in the code in part 1)
proc.h
```
// Per-process state
struct proc {
  ...
  int tickets;                 // cs202: Number of tickets the process is holding
  int stride;
  int pass;
};
```

proc.c:
```
static struct proc* allocproc(void) {
    ...
  found:
    // cs202: init the number of tickets of the new process
    p->tickets = 10;
    p->pass = 0;
    ...
}
```
```
scheduler(void)
{
  struct proc *p;
  struct cpu *c = mycpu();
  c->proc = 0;
  
  for(;;){
    // Enable interrupts on this processor.
    sti();

    // cs202: find the runable process with minimum pass
    int chosen_pass = MAX_STRIDE;
    int chosen_pid = 0;
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
      if(p->state != RUNNABLE)
        continue;
      if (chosen_pass > p->pass) {
        chosen_pass = p->pass;
        chosen_pid = p->pid;
      }
    }

    // Loop over process table looking for process to run.
    acquire(&ptable.lock);
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->state != RUNNABLE)
        continue;
      if(p->pid != chosen_pid)
        continue;

      // cs202
      p->pass += p->stride;
      
      c->proc = p;
      switchuvm(p);
      p->state = RUNNING;

      swtch(&(c->scheduler), p->context);
      switchkvm();
      c->proc = 0;
    }
    release(&ptable.lock);
  }
}
```
```
// cs202 part2
// a system call that assign tickets number for a process
void assigntickets(int num) {
  acquire(&ptable.lock);
  myproc()->tickets = num;
  release(&ptable.lock);
}
```

sysproc.c
```
// cs202
int sys_assigntickets(void) {
  count++;
  int num;
  argint(0, &num);
  assigntickets(num);
  return 0;
}
```

### Test Results
#### Case 1: [30, 20, 10]
![alt text](https://github.com/yidiwang21/cs202/blob/master/lab1/figs/part2_demo_case2.png?raw=true)

Expected scheduling ratio 3:2:1
From the above output screenshot, we can see that the relative ticks of processes are [160, 107, 53], which can be approximated to 3:2:1.

#### Case 2: [250, 100, 50]
![alt text](https://github.com/yidiwang21/cs202/blob/master/lab1/figs/part2_demo_case1.png?raw=true)

Expected scheduling ratio 5:2:1
From the above output screenshot, we can see that the relative ticks of processes are [160, 66, 33], which can be approximated to 5:2:1.

### Scheduler Performance Analysis
