INCLUDES := -I../include

%.o: %.c
	$(CC) $(DEFS) $(CFLAGS) $(INCLUDES) -c $<

.PHONY: clean

all: init.o main.o

clean:
	rm -rf *~ *.o


include ../include.mk

