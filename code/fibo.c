int fibo(int n) 
{
	if(n==0)
	{
		return 0;
	}
	int a=0,b=1;
	{
		int i;
		for(i=1;i<=n;i++)
		{
			int temp=b;
			b=a+b;
			a=temp;
		}
	}
	return b;
}
