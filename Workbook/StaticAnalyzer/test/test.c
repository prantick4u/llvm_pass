#include <string.h>

void f(char*);

void myfunc()
{
    char mbff[20];
    f(mbff);
}

void f(char *input) {
    char buf[10];
    strcpy(&buf[3], input); // unsafe

}
