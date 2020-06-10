CMAPINC = ../libcmap/
CMAPLIB = ${CMAPINC}

INCS = -I${CMAPINC}
LIBS = -L${CMAPLIB} -lcmap

CFLAGS = -Ofast -march=native -Wall -Wextra -std=c99 -pedantic

all: view

rw: rw.c
	gcc rw.c ${INCS} ${CFLAGS} ${LIBS} -o rw

walk.ff: rw
	./rw

view: walk.ff
	feh walk.ff
