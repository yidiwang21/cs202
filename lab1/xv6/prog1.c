#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {

    assigntickets(30); // write your own function here
    // printf(1, "# prog1 stride: %d\n\n", getstride());

    int i,k;
    const int loop=36000;
    for(i=0;i<loop;i++) {
        asm("nop");       //in order to prevent the compiler from optimizing the for loop
        if(i % 300 == 0) {
            printf(1, "1 ");
        } 
        for(k=0;k<loop;k++) {
            asm("nop");
        }
    }

    printf(1, "\n====== prog1 finished ======\n");
    exit();
}