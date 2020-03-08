#include <stdio.h> 
#include <stdlib.h> 
#include <strings.h> 
#include <stdint.h> 
#include <stdarg.h> 
#include <errno.h> 
#include "../wm_mem.h" 
#include "../wm_hash.h" 
#include "../wm_hex_print.h"


void testSimpleReqMem(void){
    wmHashNew(myhash);
    printfMem(memDataPointToRcEle(myhash),128);
    printfMem(memDataPointToRcEle(myhash),128);
}

void HashEmptyCallBack(char *key){
    printf("Empty Hash Set Hash[%s]\n",key);
};
void SuccessHasKeyCallBack(char *key){
    printf(">> Hash ReSet Hash[%s]\n",key);
}
void SuccessNotHasKeyCallBack(char *key){
    printf(">>Hash New Hash[%s]\n",key);
}



void testHashSet(void){
    wmHashNew(myhash);

    setWmHash_HashEmptyCallBack = HashEmptyCallBack;
    setWmHash_SuccessHasKeyCallBack = SuccessHasKeyCallBack;
    setWmHash_SuccessNotHasKeyCallBack = SuccessNotHasKeyCallBack;

    setWmHash(myhash,"A","00000000000",10);
    setWmHash(myhash,"B","00000000000",10);
    printfMem(memDataPointToRcEle(myhash),128);
    // printfMem(myhash,128);
    // setWmHash(myhash,"B","0123",4);
    // printfMem(memDataPointToRcEle(myhash),128);
}

void testReqMem(void){
    void *startP;
    uint8_t index;
    startP = reqMem(1); 
    printfMem(startP,128);

    for(index=0;index<10;index++){
        reqMem(3); 
        printfMem(startP,128);
    }


}

void test_getWmHash_NoKeyErrorCallback(char *key){
    printf("Hash dont has key:%s\n",key);
}
void test_getWmHash_EmptyErrorCallback(char *key){
    printf("Hash Empty dont has key:%s\n",key);
}
void test_getWmHash_MeetKeyCallback(char *key){
    printf("Hash has key:%s\n",key);
}

void testHashSetGet(void){
    wmHashNew(myhash);

    setWmHash_HashEmptyCallBack = HashEmptyCallBack;
    setWmHash_SuccessHasKeyCallBack = SuccessHasKeyCallBack;
    setWmHash_SuccessNotHasKeyCallBack = SuccessNotHasKeyCallBack;

    getWmHash_NoKeyErrorCallback = test_getWmHash_NoKeyErrorCallback;
    getWmHash_EmptyErrorCallback = test_getWmHash_EmptyErrorCallback;
    getWmHash_MeetKeyCallback = test_getWmHash_MeetKeyCallback;

    setWmHash(myhash,"A","0123",4);
    printfMem(memDataPointToRcEle(myhash),128);
    printf("length %d - value:%s\n",memDataPointToRcEle(myhash->valuePoint)->length,(char *)getWmHash(myhash,"A"));

    printfMem(myhash->keyPoint,128);
    printfMem(myhash->valuePoint,128);
    printf("- value:%s\n",(char *)(myhash->valuePoint));
    // printfMem(myhash,128);
    // setWmHash(myhash,"B","0123",4);
    // printfMem(memDataPointToRcEle(myhash),128);
}

void testHashSetGetDel(void){
    char *cp;
    char *rp;
    wmHashNew(myhash);

    setWmHash_HashEmptyCallBack = HashEmptyCallBack;
    setWmHash_SuccessHasKeyCallBack = SuccessHasKeyCallBack;
    setWmHash_SuccessNotHasKeyCallBack = SuccessNotHasKeyCallBack;

    getWmHash_NoKeyErrorCallback = test_getWmHash_NoKeyErrorCallback;
    getWmHash_EmptyErrorCallback = test_getWmHash_EmptyErrorCallback;
    getWmHash_MeetKeyCallback = test_getWmHash_MeetKeyCallback;

    cp = randString(rand()%20);
    setWmHash(myhash,"A",cp,strlen(cp));
    printfMem(memDataPointToRcEle(myhash),128);
    rp = getWmHash(myhash,"A");
    printf("SET:%s \nGET:%s\n",cp,rp);

    delWmHash(myhash,"A");
    rp = getWmHash(myhash,"A");
    printf("AFTER DEL\nSET:%s \nGET:%s\n",cp,rp);
    // printfMem(myhash,128);
    // setWmHash(myhash,"B","0123",4);
    // printfMem(memDataPointToRcEle(myhash),128);
}

void test_mallocErrorCallback(uint16_t index){
    printf("Mem Block[%d] Malloc Error!\n",index);
}
void test_reqMemErrorCallback(uint16_t index){
    printf("Mem Block[%d] reqMem Error!\n",index);
}
void test_reqActiveCallback(uint16_t index){
    printf("Mem Block[%d] req active block!\n",index);
}
void test_reqNULLCallback(uint16_t index){
    printf("Mem Block[%d] req null block!\n",index);
}
void test_reqRecvCallback(uint16_t index){
    printf("Mem Block[%d] req recv blockE!\n",index);
}
void test_fullBlockErrorCallback(uint16_t index){
    printf("Mem Block[%d] full Error!\n",index);
}
void test_freeBlockCallback(uint16_t index){
    printf("Mem Block[%d] free !\n",index);
}
void test_delErrorCallback(uint16_t index ){
    printf("Mem Block[%d] del Error!\n",index);
}

void test_delWmHash_EmptyCallback(char *key){
    printf("Hash[%s] del Empty!\n",key);
}
void test_delWmHash_SuccessCallback(char *key){
    printf("Hash[%s] del success!\n",key);
}
void test_delWmHash_NoKeyCallback(char *key){
    printf("Hash[%s] dont exist!\n",key);
}

void testHashSetGetLoop(void){
    wmHashNew(myhash);
    char key[]="A";
    uint8_t data[1024]="0000000000000000000000";
    uint8_t index;
    char *getChar;


    // setWmHash_HashEmptyCallBack = HashEmptyCallBack;
    // setWmHash_SuccessHasKeyCallBack = SuccessHasKeyCallBack;
    // setWmHash_SuccessNotHasKeyCallBack = SuccessNotHasKeyCallBack;

    // getWmHash_NoKeyErrorCallback = test_getWmHash_NoKeyErrorCallback;
    // getWmHash_EmptyErrorCallback = test_getWmHash_EmptyErrorCallback;
    // getWmHash_MeetKeyCallback = test_getWmHash_MeetKeyCallback;

    // delWmHash_EmptyCallback = test_delWmHash_EmptyCallback;
    // delWmHash_SuccessCallback = test_delWmHash_SuccessCallback;
    // delWmHash_NoKeyCallback = test_delWmHash_NoKeyCallback;

    // mallocErrorCallback = test_mallocErrorCallback;
    // reqMemErrorCallback = test_reqMemErrorCallback;
    // reqActiveCallback = test_reqActiveCallback;
    // reqNULLCallback = test_reqNULLCallback;
    // reqRecvCallback = test_reqRecvCallback;
    // fullBlockErrorCallback = test_fullBlockErrorCallback;
    freeBlockCallback = test_freeBlockCallback;
    delMemErrorCallback = test_delErrorCallback;

    key[0] = 'A';
    for(index=0;index<10;index++){
        // setWmHash(myhash,key,data,10);
        getChar = randString(rand()%10);
        printf("SET HASH[%s] = %s\n",key,getChar);
        setWmHash(myhash,key,getChar,strlen(getChar));
        key[0] += 1;
    }
    
    printfMem(memDataPointToRcEle(myhash),128);
    // printfMem(myhash,128);
    // setWmHash(myhash,"B","0123",4);
    // printfMem(memDataPointToRcEle(myhash),128);
    
    printf(" ---- Get Hash ----------\n");

    key[0] = 'A';
    for(index=0;index<13;index++){
        getChar = getWmHash(myhash,key);
        if(!getChar){
            key[0] += 1;
            continue;
        }
        memcpy(data,getChar,memDataPointToRcEle(getChar)->length);
        data[memDataPointToRcEle(getChar)->length] = '\0';
        printf("Get Hash[%s]->%s\n",key,data);
        key[0] += 1;
    }

    printfMem(memDataPointToRcEle(myhash),256);

    printf("----------- DELETE KEY --------------\n");
    key[0] = 'A';
    for(index=0;index<13;index++){
        delWmHash(myhash,key);
        key[0] += 1;
    }

    printf("--------  SET AGAIN --------\n");
    key[0] = 'C';
    // getChar = "abcdefghijk0123456789";
    for(index=1;index<10;index++){
        getChar = randString(rand()%20);
        printf("AGAIN SET HASH[%s] = %s\n",key,getChar);
        setWmHash(myhash,key,getChar,strlen(getChar));
        key[0] += 1;
    }

    printf(" ---- Get Hash Again ----------\n");

    key[0] = 'A';
    for(index=0;index<13;index++){
        getChar = getWmHash(myhash,key);
        if(!getChar){
            printf("Get Hash[%s] Again -> NULL\n",key);
            key[0] += 1;
            continue;
        }
        memcpy(data,getChar,memDataPointToRcEle(getChar)->length);
        data[memDataPointToRcEle(getChar)->length] = '\0';
        printf("Get Hash[%s] Again ->%s\n",key,data);
        key[0] += 1;
    }

    printf("----------- DELETE KEY Again--------------\n");
    key[0] = 'A';
    for(index=0;index<40;index++){
        delWmHash(myhash,key);
        key[0] += 1;
    }

}


int main(void){
    // printf("%s\n",(char *)StrToHex((uint8_t *)"0123456ABC",10,' '));
    // testSimpleReqMem();
    // testHashSet();
    // testHashSetGet();

    testHashSetGetLoop();

    // testReqMem();

    // testHashSetGetDel();
}