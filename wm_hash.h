#ifndef __WM__HASH__x__
#define __WM__HASH__x__
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdint.h> 
#include <stdarg.h> 
#include <errno.h> 
#include "./wm_mem.h" 
#include "./wm_hex_print.h"

#define WmHashCallback(funcName,key)\
    if(funcName){\
        funcName(key);\
    }

#define wmHashNew(NAME) \
    wmHashNode *NAME;\
    NAME = (wmHashNode *)wmMemDataPoint(reqMem(sizeof(wmHashNode)));\
    NAME->keyPoint = NULL;\
    NAME->valuePoint =NULL;\
    NAME->nextPoint = NULL; 

struct __wmHashNode__; 
typedef struct __wmHashNode__ wmHashNode; 
struct __wmHashNode__{ 
    char *keyPoint; 
    void *valuePoint; 
    wmHashNode * nextPoint; 
    // wmHashNode * prePoint; 
}; 

int hashKeyEq(char *key,wmHashNode *node){ 
    static uint16_t index; 
    static uint16_t key_len; 
    static uint16_t hash_key_len; 
    static char *hash_key_point; 
    // reqMemRcEle *rcP;
    key_len=strlen(key);
    // rcP = memDataPointToRcEle(node->keyPoint); 
    // printf("--- P: %p ---\n",rcP); 
    // printf("--- P: %p ---\n",rcP); 
    // printf("--- P: %d ---\n",(rcP->contect).length); 
    hash_key_len = (memDataPointToRcEle(node->keyPoint))->length; 
    if(key_len != hash_key_len){ 
        return 0; 
    }else{ 
        hash_key_point = node->keyPoint; 
        for(index=0;index<key_len;index++){ 
            if(key[index] != hash_key_point[index]){
                 return 0; 
            } 
        } 
    } return 1; 
} 

// setHashCallBack

void (* setWmHash_HashEmptyCallBack)(char *key);
void (* setWmHash_SuccessHasKeyCallBack)(char *key);
void (* setWmHash_SuccessNotHasKeyCallBack)(char *key);

void *setWmHash(wmHashNode *port,char *key,void *value,uint16_t value_len){ 
    static int rel; 
    static reqMemRcEle *ele; 
    static reqMemRcEle *key_ele; 
    static reqMemRcEle *value_ele; 
    // static wmHashNode *node_tmp; 
    // if(!key || !value || !port){
         // return NULL; 
    // } 
    if(port->keyPoint==NULL && port->nextPoint==NULL){
        // printf("----PP---%ld-\n",strlen(key));

        WmHashCallback(setWmHash_HashEmptyCallBack,key);

        key_ele = reqMem(sizeof(char) * strlen(key)); 
         // printf("----PP----\n"); 
        if(! memcpy(wmMemDataPoint(key_ele),key,sizeof(char) * strlen(key))){
              printf(" memcpy ERROR"); return NULL; 
        }
        // printf("-------------------KEY------------------------\n");
        // printfMem(key_ele,128);
        // 分配value 
        value_ele = reqMem(value_len); 
        if(! memcpy(wmMemDataPoint(value_ele),value,value_len)){
             printf(" memcpy ERROR"); return NULL; 
        } 
        // printf("-------------------POST VALUE------------------------\n");
        // printfMem(key_ele,128);
        //分配 Node 
        // ele = reqMem(sizeof(wmHashNode)); 
        // node_tmp = (wmHashNode *)(wmMemDataPoint(ele)); 
        port->nextPoint = NULL; 
        port->keyPoint = (char *)wmMemDataPoint(key_ele); 
        port->valuePoint = wmMemDataPoint(value_ele); 
        printf("KP:%p VP%p\n",key_ele,value_ele); 
        return port->valuePoint; 
    } 
    
    if(key==port->keyPoint && key){ 
        WmHashCallback(setWmHash_SuccessHasKeyCallBack,key);
        ele = reqMem(value_len); 
        memcpy(wmMemDataPoint(ele),value,value_len); 
        port->valuePoint = ele; 
        return ele; 
    } 
    rel = hashKeyEq(key, port); 
    if(rel){ 
        // printf("EQ Assign \n"); 
        value_ele = reqMem(value_len); 
        memcpy(wmMemDataPoint(value_ele),value,value_len); 
        port->valuePoint = wmMemDataPoint(value_ele); 
        return port->valuePoint; 
    }else{ 
        if(!(port->nextPoint)){
            WmHashCallback(setWmHash_SuccessNotHasKeyCallBack,key);
            // NEXT NULL;; 
            //分配 Node 
            // printf("NULL -NEXT\n");
            ele = reqMem(sizeof(wmHashNode)); 
            // reqMem(sizeof(wmHashNode));
            port->nextPoint = (wmHashNode *)(wmMemDataPoint(ele)); 
            // 分配KEY 
            key_ele = reqMem(sizeof(char) * strlen(key)); 
            memcpy(wmMemDataPoint(key_ele),key,sizeof(char) * strlen(key)); 
            // 分配value 
            value_ele = reqMem(value_len); 
            memcpy(wmMemDataPoint(value_ele),(uint8_t *)value,value_len); 
            port->nextPoint->nextPoint = NULL; 
            port->nextPoint->keyPoint = (char *)wmMemDataPoint(key_ele); 
            port->nextPoint->valuePoint = wmMemDataPoint(value_ele); 
            // printf("BBBBB>> %s:%p \n",key,port->nextPoint->valuePoint);
            return port->nextPoint->valuePoint; 
        }else{ 
            return setWmHash(port->nextPoint,key,value,value_len);
        } 
    } 
} 

void (* getWmHash_NoKeyErrorCallback)(char *key);
void (* getWmHash_EmptyErrorCallback)(char *key);
void (* getWmHash_MeetKeyCallback)(char *key);

void *getWmHash(wmHashNode *port,char *key){ 
    static int rel; 
    // if(!key || !value || !port){ 
        // return NULL; 
        // } 
    if(port->keyPoint==NULL && port->nextPoint==NULL){ 
        WmHashCallback(getWmHash_EmptyErrorCallback,key);
        return NULL; 
    } 
    if(key==port->keyPoint && key){ 
        WmHashCallback(getWmHash_MeetKeyCallback,key);
        return port->valuePoint; 
    } 
    rel = hashKeyEq(key, port); 
    // printf(">>%s,%s,%d\n",key,port->keyPoint,rel); 
    // printf(">>%ld,%ld,%d\n",strlen(key),strlen(port->keyPoint),rel); 
    // printf(">> string lem %d\n",memDataPointToRcEle(port->keyPoint)->contect.length); 
    if(rel){ 
        WmHashCallback(getWmHash_MeetKeyCallback,key);
        return port->valuePoint; 
    }else{ 
        if(!(port->nextPoint)){ 
            // NEXT NULL; 
            WmHashCallback(getWmHash_NoKeyErrorCallback,key);
            return NULL; 
        }else{ 
            return getWmHash(port->nextPoint,key); 
        }
    } 
} 

void (* delWmHash_EmptyCallback)(char *key);
void (* delWmHash_SuccessCallback)(char *key);
void (* delWmHash_NoKeyCallback)(char *key);

int delWmHashCore(wmHashNode *port,char *key,wmHashNode *prePort){
    // if(!key || !value || !port){ 
        // return NULL; 
        // } 
    if(port->keyPoint==NULL && port->nextPoint==NULL){ 
        WmHashCallback(delWmHash_EmptyCallback,key);
        return 0; 
    } 
    if(key==port->keyPoint && key){ 
        // delMem() 
        delMem( memDataPointToRcEle(port->keyPoint) ); 
        delMem( memDataPointToRcEle(port->valuePoint) ); 
        WmHashCallback(delWmHash_SuccessCallback,key);
        //if pre NULL 
        if(! prePort){ 
            // next NULL 
            if( !port->nextPoint){ 
                port->keyPoint = NULL; port->valuePoint = NULL;
            }else{ 
                port->keyPoint = port->nextPoint->keyPoint; 
                port->valuePoint = port->nextPoint->valuePoint; 
                port->nextPoint = port->nextPoint->nextPoint; 
                delMem( memDataPointToRcEle(port->nextPoint) ); 
            } 
        }else{ 
            prePort->valuePoint = port->nextPoint; 
            delMem( memDataPointToRcEle(port)); 
        } 
            
        return 1; 
    } 
    if(hashKeyEq(key, port)){ 
        delMem( memDataPointToRcEle(port->keyPoint) ); 
        delMem( memDataPointToRcEle(port->valuePoint) ); 
        //if pre NULL 
        if(! prePort){ 
            // next NULL 
            if( !port->nextPoint){ 
                port->keyPoint = NULL;
                port->valuePoint = NULL; 
            }else{ 
                port->keyPoint = port->nextPoint->keyPoint; 
                port->valuePoint = port->nextPoint->valuePoint; 
                delMem( memDataPointToRcEle(port->nextPoint) ); 
                port->nextPoint = port->nextPoint->nextPoint; 
            } 
        }else{ 
            prePort->nextPoint = port->nextPoint; 
            delMem( memDataPointToRcEle(port) ); 
        } 
        WmHashCallback(delWmHash_SuccessCallback,key);
        return 1; 
    }else{ 
        if(!(port->nextPoint)){ 
            WmHashCallback(delWmHash_NoKeyCallback,key);
            // NEXT NULL; 
            return 0; 
        }else{ 
            return delWmHashCore(port->nextPoint,key,port); 
        } 
    } 
} 
        
int delWmHash(wmHashNode *port,char *key){ 
    // printf("%s",key); 
    wmHashNode *prePort=NULL; 
    return delWmHashCore(port,key,prePort); 
    // return 1; 
} 
#endif