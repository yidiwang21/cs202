#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"


// TODO: finish syscalls for clone()
void *thread_create(void*(*start_routine)(void*), void *arg) {
    void *stack = malloc(2 * KSTACKSIZE);

    int tid = clone(start_routine, stack, 0, arg);

    return 0;
}