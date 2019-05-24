#include "lib.h"
#define TMPVA ((char *) 0x50005000)
const char *msg = "hello world!\n";
const char *msg2 = "goodbye ,world!\n";

void childofspawn(void);


void umain (int argc, char **argv)
{
    int r;
    if(argc!=0)
        childofspawn();

    if((r=syscall_mem_alloc(0,TMPVA,PTE_R|PTE_V|PTE_LIBRARY))<0)
        user_panic("syscall error!");
    if((r=fork())<0)
        user_panic("fork: %d",r);
    if(r==0){
        strcpy(TMPVA,msg);
        exit(); 
    }   

    wait(r);
    writef("TMPVAis  %s \n",TMPVA);
    if(strcmp(TMPVA,msg)==0)
        writef("fork solved the problem of PTE_LIBRARY,congratulations!\n");
    else
        user_panic("sorry , your fork may be wrong\n");
    if((r=spawnl("testptelibrary.b","testptelibrary","arg",0))<0)
        user_panic("spawn wrong: %d",r);
    wait(r);
    if(strcmp(TMPVA,msg2)==0)
        writef("spawn solved the problem of PTE_LIBRARY,congratulations!\n");
    else
        user_panic("sorry , your spawn may be wrong\n");


}

void childofspawn(void){
    strcpy(TMPVA,msg2);
    exit();
}
