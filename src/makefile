AS=as
CC=gcc
LD=ld

CFLAGS = -Wall -O0

ctest:ctest.o
	${CC} -o $@ $<
atest:atest.o
	${LD} -o $@ $<
clean:
	rm -f *.o *~ ctest atest

.c.o:
	${CC} -c -o $@ $< ${CFLAGS}
.s.o:
	${AS} -o $@ $<
