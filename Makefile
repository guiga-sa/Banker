all:banker.c
	gcc -o banker banker.c
clean:
	rm banker