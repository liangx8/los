GCC=gcc
AS=as
OBJS=boot.o kernel.o vga.o
CFLAGS=-std=gnu99
CFLAGS+=-ffreestanding
CFLAGS+=-Wall
CFLAGS+=-Wextra
CFLAGS+=-m32
GLD=ld

all:kernel.elf
	@echo "*** Done ***"
kernel.elf:${OBJS}
#	${GCC} -T linker.ld -m32 -o $@ -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc
	${GCC} -T linker.ld -m32 -o $@ -ffreestanding -nostdlib ${OBJS}
clean:
	rm -f ${OBJS} *~ kernel.elf a c *.o

.s.o:
	${AS} -32 $< -o $@
.c.o:
	${GCC} -c $< -o $@ ${CFLAGS}



c:ctest.o casm.o
	${GCC} -o $@ ctest.o casm.o
a:atest.o
	${GLD} -o $@ $<

casm.o:casm.s
	${AS} -o $@ $<
ctest.o:ctest.c
	${CC} -c -o $@ $< 
atest.o:atest.s
	${AS} -o $@ $<
