AS=as
CC=gcc
LD=ld

CFLAGS = -Wall -O0

c:ctest.o
	${CC} -o $@ $<
a:atest.o
	${LD} -o $@ $<
clean:
	rm -f *.o *~ c a

.c.o:
	${CC} -c -o $@ $< ${CFLAGS}
.s.o:
	${AS} -o $@ $<
