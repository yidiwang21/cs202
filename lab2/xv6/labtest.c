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
    sleep(40);
    
    // FIXME:
    printf(1, "# Simulation of Frisbee game has finished, %d rounds were played in total!\n", pass_round);
    exit();
}