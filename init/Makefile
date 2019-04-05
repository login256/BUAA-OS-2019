INCLUDES := -I../include

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

.PHONY: clean

all: init.o main.o code_a.o code_b.o

clean:
	rm -rf *~ *.o


include ../include.mk

