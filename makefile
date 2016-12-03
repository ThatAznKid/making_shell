startup: main.c
	gcc main.c helper.c

run: a.out
	./a.out

clean: a.out 
	rm a.out