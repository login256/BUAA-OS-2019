INCLUDES := -I../include

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

.PHONY: clean

all: init.o main.o code.o

clean:
	rm -rf *~ *.o


include ../include.mk

