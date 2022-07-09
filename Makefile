CFLAGS = -std=c11

all:
	gcc $(CFLAGS) -o main main.c

clean:
	del main.exe