GCC=gcc
AS=as
OBJS=boot.o kernel.o vga.o
CFLAGS=-std=gnu99 -ffreestanding -Wall -Wextra -m32
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



c:ctest.o
	${GCC} -o $@ $<
a:atest.o
	${GLD} -o $@ $<


ctest.o:ctest.c
	${CC} -c -o $@ $< ${CFLAGS}
atest.o:atest.s
	${AS} -o $@ $<
