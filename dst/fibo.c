#include<stdio.h>

int printFibo(int n){
	int a = 0;
	int b = 1;
	{
		int i;
		for(i=1; i<=n; i++)
		{
			int temp = b; 
			printf("%d ",b);
			b = a + b;
			a = temp;
		}
	}
	return;
}

int main(){
	int var;
	scanf("%d", &var);
	printFibo(var);
	return 0;
}
