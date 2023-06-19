#include <ccall.h>
#include <stdio.h>
#include "../include/x86call.h"

void ccall(char *str){
    printf("%s\n", str);
    int foo = x86call(4);
    printf("%d\n", foo);
}
