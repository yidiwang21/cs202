#include "types.h"
#include "stat.h"
#include "user.h"

void ticks(char* prog_name, int cnt) {
    int i, k;
    const int loop = 48000;
    for(i = 0; i < loop; i++) {
        asm("nop");  
        if(i % 300 == 0) {
            cnt++;
            printf(1, "# %s ticks: %d\n", prog_name, cnt);
            // printf(1, "============ Sum = %d ============\n", getstride());
        }     
        for(k = 0; k < loop; k++) asm("nop");
    }
}

int main(int argc, char *argv[]) {
    int cnt1 = 0;
    int cnt2 = 0;
    int cnt3 = 0;

    setsumtickets(400);

    int pid1 = fork();

    if (pid1 == 0) {
        assigntickets(50);
        ticks("prog3", cnt3);
        exit();
            
    }else if (pid1 > 0) {    // child 1 process

        int pid2 = fork();
        if (pid2 == 0) {
            assigntickets(100);
            ticks("prog2", cnt2);    
        
        }else if(pid2 > 0) {
            assigntickets(250);
            
            ticks("prog1", cnt1);

            

            kill(pid2);
            kill(pid1);
            printf(1, "============ prog1 is finished ============\n");
            
            exit();
        
        
        }else {
            printf(1, "# fork(2) error.\n");
            exit();
        }
    }else {
        printf(1, "# fork(1) error.\n");
        exit();
    }


}