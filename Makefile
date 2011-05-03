ofiles=utilities.o database.o interface.o main.o
stdlibs=-lform -lcurses -lm
adilibs=-ldb

mass_linux: database.o mass.o
	cc database.o mass.o $(adilibs) -lm -o mass
mass_bsd: database.o mass.o
	cc database.o mass.o -lm -o mass
linux: $(ofiles)
	cc $(ofiles) $(adilibs) $(stdlibs) -o eis
bsd: $(ofiles)
	cc $(ofiles) $(stdlibs) -o eis
main.o: main.h main.c
	cc -c main.c -g
database.o: database.h database.c
	cc -c database.c -g
interface.o: interface.h interface.c
	cc -c interface.c -g
utilities.o: utilities.h utilities.c
	cc -c utilities.c -g
mass.o: mass.h mass.c
	cc -c mass.c -g

clean:
	rm *.o
	rm eis
