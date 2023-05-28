CC=gcc
CFLAGS=-std=gnu2x -O3 -ggdb3 -luv -Wall -Wextra -Wpedantic -Wconversion -Werror ${LIBS} ${C_EXTRA_FLAGS}

all: bulky

clean:
	rm *.o bulky

bulky: main.c request.o response.o
	${CC} ${CFLAGS} $^ -o $@

request.o: request.c
	${CC} ${CFLAGS} -c $< -o $@

response.o: response.c
	${CC} ${CFLAGS} -c $< -o $@
