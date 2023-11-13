CC=gcc
CFLAGS= -Wall -I ./include
LDFLAGS =-L ./lib -lmingw -lSDL2main -lSDL2  -lm
PROGRAMS= app



all: $(PROGRAMS)

app:mon_main.o graphicSDL.o vaisseau.o asteroides.o missile.o
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)


mon_main.o :./src/mon_main.c ./include/vaisseau.h ./include/asteroides.h ./include/graphicSDL.h ./include/missile.h
	$(CC) -c $(CFLAGS) src/mon_main.c -lm

vaisseau.o:./src/vaisseau.c ./include/vaisseau.h
	$(CC) -c $(CFLAGS) src/vaisseau.c -lm

missile.o:./src/missile.c ./include/missile.h 
	$(CC) -c $(CFLAGS) src/missile.c -lm

asteroides.o:./src/asteroides.c ./include/asteroides.h
	$(CC) -c $(CFLAGS) src/asteroides.c -lm

graphicSDL.o:./src/graphicSDL.c ./include/graphicSDL.h
	$(CC) -c $(CFLAGS) src/graphicSDL.c -lm


clean:
	rm -f *.o *~ $(PROGRAMS)