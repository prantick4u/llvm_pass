#include <stdio.h>

void __log_access(void *ptr) {
    printf("🔍 Access at address: %p\n", ptr);
}

/*
int main() {
    int x = 42;
    __log_access(&x);
    return 0;
}
*/

