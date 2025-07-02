#include <string.h>

void f(char *input) {
    char buf[10];
    strcpy(buf, input); // Unsafe!
    strcpy(buf, input); // Unsafe!

}
