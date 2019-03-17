INCLUDES := -I./ -I../ -I../include/
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

%.o: %.S
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

.PHONY: clean

all: print.o printf.o env.o genex.o

clean:
	rm -rf *~ *.o


include ../include.mk
