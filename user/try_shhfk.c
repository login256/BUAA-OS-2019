#include "lib.h"

int a[100000];
void umain()
{
	int b[10000];
	b[0]=8;
	a[0]=8;
	if(fork()==0){
		b[0]=7;
		a[0]=7;
		if(fork()==0){
			b[0]=6;
			a[0]=6;
			if(fork()==0){
				b[0]=5;
				a[0]=5;
				if(fork()==0){
					b[0]=4;
					a[0]=4;
					if(fork()==0){
						b[0]=3;
						a[0]=3;
						if(fork()==0){
							b[0]=2;
							a[0]=2;
							if(fork()==0){
								b[0]=1;
								a[0]=1;
							}
						}
					}
				}
			}

		}
	}
	int id=fork();
	if(id==0){
		b[1]=10;
		a[1]=20;
	}
	id=fork();
	if(id==0){
		b[2]=30;
		a[2]=40;
	}
	writef("0x%x,with a0:0x%X,b0:0x%x,a1:0x%x,b1:0x%x,a2:0x%x,b2:0x%x\n",syscall_getenvid(),a[0],b[0],a[1],b[1],a[2],b[2]);
}
