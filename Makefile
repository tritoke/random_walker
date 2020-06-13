CMAPINC = libcmap
CMAPLIB = ${CMAPINC}

INCS = -I${CMAPINC}
LIBS = -L${CMAPLIB} -lcmap

CFLAGS = -Ofast -march=native -Wall -Wextra -std=c99 -pedantic

all: view

1: 1rw view
1rw: 1/rw.c
	gcc 1/rw.c ${INCS} ${CFLAGS} ${LIBS} -o rw

2: 2rw view
2rw: 2/rw.c
	gcc 2/rw.c ${INCS} ${CFLAGS} ${LIBS} -o rw

3: 3rw view
3rw: 3/rw.c
	gcc 3/rw.c ${INCS} ${CFLAGS} ${LIBS} -o rw

4: 4rw view
4rw: 4/rw.c
	gcc 4/rw.c ${INCS} ${CFLAGS} ${LIBS} -o rw

clean:
	rm -f rw

walk.ff: rw
	./rw

view: walk.ff
	feh walk.ff

png: walk.ff


.PHONY: all clean view 1 2 3
