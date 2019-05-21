#include "user.h"
#include "types.h"

int pass_no = 0;
int tid = 0;
int pass_round = 0;
lock_t lock;

void func(void* arg) {
    lock_acquire(&lock);
    if (pass_no <= pass_round) {
        pass_no++;
        printf(1, "# Pass number no: %d ", pass_no);
        lock_release(&lock);
    }else{
        lock_release(&lock);
        exit();
    }
}

void tid_increment(int n) {
    lock_acquire(&lock);
    printf(1, "Thread %d is passing the token to thread %d\n", tid % n, (tid+1) % n);
    tid++;
    lock_release(&lock);
}

int main(int argc, char *argv[]) {
    lock_init(&lock);
    int thread_num;

    if (argc != 3) {
        thread_num = 4;
        pass_round = 6;
        printf(1, "# Default thread number: 4, pass round: 6\n");
    }else {
        thread_num = atoi(argv[1]);
        pass_round = atoi(argv[2]);
    }

    // create threads
    int i;
    for (i = 0; i < thread_num; i++) {
        thread_create((void*)func, (void*)0);
        tid_increment(thread_num);
    }

    printf(1, "Simulation of Frisbee game has finished, 6 rounds were played in total!\n");
    exit();
}