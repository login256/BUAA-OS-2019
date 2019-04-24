INCLUDES := -I./ -I../ -I../include/
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

%.o: %.S
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

.PHONY: clean

all: env.o print.o printf.o sched.o env_asm.o kclock.o traps.o genex.o kclock_asm.o syscall.o syscall_all.o getc.o kernel_elfloader.o

clean:
	rm -rf *~ *.o


include ../include.mk
