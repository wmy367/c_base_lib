#ifndef __WM_HEX_PRINT__
#define __WM_HEX_PRINT__
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <stdint.h>
#include <stdarg.h>

uint8_t *StrToHex(uint8_t *chs,uint16_t len,char split){
    static uint8_t array [1024*4];
    uint16_t index;
    uint8_t curr_char;
    for(index=0;index<len;index++){
        curr_char = chs[index];
        array[4*index]      = split;
        array[4*index+1]    = split;
        array[4*index+2]    = (((curr_char/16)%16) < 10) ? ((curr_char/16)%16) + 0x30 : ((curr_char/16)%16)-10 + 0x41;
        array[4*index+3]    = ((curr_char%16) < 10) ? (curr_char%16) + 0x30 : (curr_char%16)-10 + 0x41;
    } 

    array[4*index] = '\0';

    return array;
}

void printfMem(void *point,uint16_t len){
    uint8_t *cp;
    uint16_t index;
    printf(">%p\n",point);
    cp = StrToHex((uint8_t *)point,len, ' ');
    if(!point){
        printf("Point  == NULL");
    }
    // printf(">%p\n",point);
    index = 0;
    while(cp[index] != '\0'){
        printf("%c",cp[index++]);

        if(index%64==0){
            printf("\n");
        }
    }
}

char *randString(uint8_t len){
    uint8_t index;
    static char array[1024];
    char latin[]="0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@~#$%^&*()_+<>?\":{}|;";

    for(index=0;index<len;index++){
        array[index] = latin[rand()%85];
    }
    array[index] = '\0';

    return array;
}


#endif