
# ========= Configuration =========

INCLUDES	  := -I../include/

# ======== End of configuration ===

%.o: %.S
	$(CC) $(CFLAGS) $(INCLUDES) -c $< 

.PHONY: clean

all: start.o

clean:
	rm -rf *~ *.o

include ../include.mk
