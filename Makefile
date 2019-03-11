all : 
	$(MAKE) --directory="code"
	gcc ./code/main.o ./code/fibo.o -o fibo

clean :
	$(MAKE) clean --directory="code"

