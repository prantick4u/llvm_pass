#include <stdio.h>

void __log_access(void *addr, char isStore, int size, const char *func, const char *file, int line) {
    printf("🔍 %s of %d bytes at %p in %s (%s:%d)\n",
        isStore ? "Store" : "Load", size, addr, func, file, line);
}


