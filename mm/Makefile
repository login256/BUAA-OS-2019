INCLUDES	:= -I./ -I../ -I../include/

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

%.o: %.S
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

.PHONY: clean

all: pmap.o tlb_asm.o

clean:
	rm -rf *~ *.o


include ../include.mk
