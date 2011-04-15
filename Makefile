eis: database.o main.o
	cc database.o main.o -ldb -lm -o eis
main.o: main.h main.c
	cc -c main.c -g
database.o: database.h database.c
	cc -c database.c -g

clean:
	rm *.o
	rm eis
