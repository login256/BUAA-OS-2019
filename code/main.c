#include<stdio.h>
#include"fibo.h"

int main()
{
    int i,x;
    scanf("%d", &x);
    for(i = 1; i <= x; i++){
        printf("%d ", fibo(i));
    }
    return 0;
}


