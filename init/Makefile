INCLUDES := -I../include

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<
%.o: %.S
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

.PHONY: clean

all: init.o main.o code_a.o code_b.o init_asm.o

clean:
	rm -rf *~ *.o


include ../include.mk

