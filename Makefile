eis: utilities.o database.o interface.o main.o
	cc utilities.o database.o interface.o main.o -ldb -lform -lcurses -lm -o eis
main.o: main.h main.c
	cc -c main.c -g
database.o: database.h database.c
	cc -c database.c -g
interface.o: interface.h interface.c
	cc -c interface.c -g
utilities.o: utilities.h utilities.c
	cc -c utilities.c -g

clean:
	rm *.o
	rm eis
