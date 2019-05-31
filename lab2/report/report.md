# CS202 Lab 2 Report
Yidi Wang </br>
862114701

## Modifications
### clone() System Call
The clone() system call is written based on fork() system call.

#### Address Space
Different from fork() system call, clone() system call uses parent's address space.
```
np->state = UNUSED;
np->sz = currproc->sz;
np->parent = currproc;
*np->tf = *currproc->tf;
np->pgdir = currproc->pgdir;
```

#### File Descriptor
The thread should use the same file descriptor as parent.
```
for (i = 0; i < NOFILE; i++)
    if (currproc->ofile[i])
        np->ofile[i] = filedup(currproc->ofile[i]);
np->cwd = idup(currproc->cwd);
safestrcpy(np->name, currproc->name, sizeof(currproc->name));
```

#### User Stack


### thread_create() Function


### Spinlock


## Test