#include <stdlib.h>

int main() {
    system("gatttool -b 96:90:16:63:2D:25 --char-write-req -a 0x0010 -n 6b7570");
    return 0;
}

