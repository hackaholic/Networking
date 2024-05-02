/*
   Author: Kumar Shubham

   This program detets that the host byte ordering
*/


#include <stdio.h>


int main(int argc, char *argv[]) {

    short s = 0x1234;
    char *p = (char *) &s;

    printf("lowest memory value %x,  heighest memory val: %x\n", *p, *(p+1));


    if(*p == 0x12) {
        printf("Big-Endian\n");  
    }
    else {
        printf("Little Endian\n");
    }
    return 0;
}