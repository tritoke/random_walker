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

clean:
	rm -f rw

rw:
	@printf "Hi, please choose which project to make:\n"
	@printf "\tmake (1|2|3)\n"

walk.ff: rw
	[ -f ./rw ] && ./rw

view: walk.ff
	feh walk.ff

png: walk.ff


.PHONY: all clean view rw 1 2 3
