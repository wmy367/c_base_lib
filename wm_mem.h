
#ifndef __WM__MEM__
#define __WM__MEM__
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdint.h> 
#include <stdarg.h> 
#include <errno.h> 

#define     WM_MEM_BLOCK_TOTAL  4
#define     WM_MEM_BLOCK_SIZE   256

#define     WM_BMEM_NULL       0
#define     WM_BMEM_ACTIVE     1
#define     WM_BMEM_RECOV      2

#define WmMemCallback(funcName,index)\
    if(funcName){\
        funcName(index);\
    }

typedef struct{
    void        *mallocAddr;
    uint16_t    incAddr;
    uint16_t    delCollect;
    uint8_t     status;      
} WmMemBlock;

static WmMemBlock  memBlockArray[WM_MEM_BLOCK_TOTAL];

typedef struct {
    // uint16_t    blockIndex;
    uint16_t    length;
} reqMemRcEle;

void (* mallocErrorCallback)(uint16_t index);
void (* reqMemErrorCallback)(uint16_t index);
void (* reqActiveCallback)(uint16_t index);
void (* reqNULLCallback)(uint16_t index);
void (* reqRecvCallback)(uint16_t index);
void (* fullBlockErrorCallback)(uint16_t index);
void (* freeBlockCallback)(uint16_t);
void (* delMemErrorCallback)(uint16_t index );

reqMemRcEle *reqMem(uint16_t size){
    uint16_t     index;
    reqMemRcEle   *rcEle;
    // 先循环查看激活的
    for(index=0;index<WM_MEM_BLOCK_TOTAL;index++){
        if(memBlockArray[index].status == WM_BMEM_ACTIVE && (memBlockArray[index].incAddr+size+sizeof(reqMemRcEle)) <=WM_MEM_BLOCK_SIZE){
            rcEle = (reqMemRcEle *)(memBlockArray[index].mallocAddr+memBlockArray[index].incAddr);
            // printf(">>>>%p offset:%d  -> %ld\n",rcEle,memBlockArray[index].incAddr,size+sizeof(reqMemRcEle));
            rcEle->length = size;
            memBlockArray[index].incAddr += (size+sizeof(reqMemRcEle));
            WmMemCallback(reqActiveCallback,index);
            return rcEle;
        }
    }
    // 循环 Recovery
    for(index=0;index<WM_MEM_BLOCK_TOTAL;index++){
        if(memBlockArray[index].status == WM_BMEM_RECOV && (size+sizeof(reqMemRcEle) )<=WM_MEM_BLOCK_SIZE){
            memBlockArray[index].mallocAddr = (void *)malloc(WM_MEM_BLOCK_SIZE*sizeof(uint8_t));
            memBlockArray[index].delCollect = 0;
            if(! memBlockArray[index].mallocAddr){
                WmMemCallback(mallocErrorCallback,index);
            }
            rcEle = (reqMemRcEle *)(memBlockArray[index].mallocAddr);
            rcEle->length = size;
            memBlockArray[index].incAddr = (size+sizeof(reqMemRcEle));
            memBlockArray[index].status = WM_BMEM_ACTIVE;
            WmMemCallback(reqRecvCallback,index);
            return rcEle;
        }
    }
    // 循环NULL
    for(index=0;index<WM_MEM_BLOCK_TOTAL;index++){
        if(memBlockArray[index].status == WM_BMEM_NULL && (size+sizeof(reqMemRcEle) )<=WM_MEM_BLOCK_SIZE){
            memBlockArray[index].mallocAddr = (void *)malloc(WM_MEM_BLOCK_SIZE*sizeof(uint8_t));
            memBlockArray[index].delCollect = 0;
            if(! memBlockArray[index].mallocAddr){
                    WmMemCallback(mallocErrorCallback,index);
            }            
            rcEle = (reqMemRcEle *)(memBlockArray[index].mallocAddr);
            rcEle->length = size;
            memBlockArray[index].incAddr = (size+sizeof(reqMemRcEle));
            memBlockArray[index].status = WM_BMEM_ACTIVE;
            WmMemCallback(reqNULLCallback,index);
            return rcEle;
        }
    }

    WmMemCallback(fullBlockErrorCallback,index);

    return NULL;
}

int delMem(reqMemRcEle *ele){
    int blockIndex = -1;
    uint16_t index;

    for(index=0;index<WM_MEM_BLOCK_TOTAL;index++){
        if(memBlockArray[index].status == WM_BMEM_ACTIVE){
            if((memBlockArray[index].mallocAddr <= (void *)ele) && ((void *)ele < memBlockArray[index].mallocAddr+WM_MEM_BLOCK_SIZE) ){
                blockIndex = index;
                break;
            }
        }
    }

    if(blockIndex >=0 ){
        memBlockArray[blockIndex].delCollect += (ele->length + sizeof(reqMemRcEle));
        // printf("SADDR:%p EleAddr:%p Eaddr: %p [%d]Curr del collect + %ld =%d / %d \n",memBlockArray[blockIndex].mallocAddr,ele,memBlockArray[blockIndex].mallocAddr+WM_MEM_BLOCK_SIZE,
        //     blockIndex,ele->length + sizeof(reqMemRcEle),memBlockArray[blockIndex].delCollect,memBlockArray[blockIndex].incAddr);
        if(memBlockArray[blockIndex].delCollect >= memBlockArray[blockIndex].incAddr){
            //释放内存
            memBlockArray[blockIndex].status = WM_BMEM_RECOV;
            memBlockArray[blockIndex].delCollect = 0;
            memBlockArray[blockIndex].incAddr = 0;
            free(memBlockArray[blockIndex].mallocAddr);
            // if(freeBlockCallback){
            //     freeBlockCallback((uint16_t )blockIndex);
            // }
            WmMemCallback(freeBlockCallback,blockIndex);
            return 1;
        }
    }else{
        printf("%p>>> %s\n",ele,(char *)((void *)ele + sizeof(reqMemRcEle)));
        WmMemCallback(delMemErrorCallback,blockIndex);
        return 0;
    }
    return 0;
}

reqMemRcEle *memDataPointToRcEle(void *point){
    return (reqMemRcEle *)(point - sizeof(reqMemRcEle));
}

void *wmMemDataPoint(reqMemRcEle *ele){
    return (void *)ele + sizeof(reqMemRcEle);
}


#endif