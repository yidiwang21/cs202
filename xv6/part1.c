#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
    int param = atoi(argv[1]);

    int ret = info(param);
    if (param == 1) {
        printf(1, "# Number of processes in the system: %d\n", ret);
    }else if (param == 2) {
        printf(1, "# Number of syscalls the process has done so far: %d\n", ret);
    }else if (param == 3) {
        printf(1, "# Number of memory pages the process is using: %d\n", ret);
    }else {
        printf(1, "# Wrong input number!");
    }

    exit();
    return 0;   // never reached
}