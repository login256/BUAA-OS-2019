INCLUDES = -I../include/ -I../user/

user_dir := ../user
tools_dir := .
USERLIB :=	$(user_dir)/printf.o \
		$(user_dir)/print.o \
		$(user_dir)/libos.o \
		$(user_dir)/fork.o \
		$(user_dir)/pgfault.o \
		$(user_dir)/syscall_lib.o \
		$(user_dir)/ipc.o \
		$(user_dir)/string.o \
		$(user_dir)/fd.o \
		$(user_dir)/pageref.o \
		$(user_dir)/file.o \
		$(user_dir)/fsipc.o \
		$(user_dir)/pipe.o \
		$(user_dir)/console.o \
		$(user_dir)/fprintf.o

FSLIB :=	fs.o \
		ide.o \
		test.o

FSIMGFILES := motd newmotd

CFLAGS += -nostdlib -static

all: serv.x fs.img serv.b

%.x: %.b.c
	echo cc1 $<
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

%.b.c: %.b
	echo create $@
	echo bintoc $* $< > $@~
	chmod +x $(tools_dir)/bintoc
	$(tools_dir)/bintoc $* $< > $@~ && mv -f $@~ $@
#	grep \. $@
	
%.b: ../user/entry.o ../user/syscall_wrap.o %.o $(USERLIB) $(FSLIB)
	echo ld $@
	$(LD) -o $@ $(LDFLAGS) -G 0 -static -n -nostdlib -T $(user_dir)/user.lds $^

%.o: %.c
	echo user1 cc $<
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.o: %.S
	echo as $<
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<


%.o: $(user_dir)/lib.h

fs.img: $(FSIMGFILES)
	dd if=/dev/zero of=../gxemul/fs.img bs=4096 count=1024 2>/dev/null
	gcc $(INCLUDES) fsformat.c -o $(tools_dir)/fsformat -m32
	chmod +x $(tools_dir)/fsformat
	$(tools_dir)/fsformat ../gxemul/fs.img $(FSIMGFILES)

.PHONY: clean

clean:
	rm -rf *~ *.o *.b.c *.b *.x

include ../include.mk
