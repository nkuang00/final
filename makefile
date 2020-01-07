all: deck.o display.o draw.o play.o main.o
	gcc -o program deck.o display.o draw.o play.o main.o

main.o: main.c play.h display.h draw.h deck.h
	gcc -c main.c

play.o: play.c play.h display.h draw.h deck.h
	gcc -c play.c

draw.o: draw.c draw.h display.h deck.h
	gcc -c draw.c

display.o: display.c display.h deck.h
	gcc -c display.c

deck.o: deck.c deck.h
	gcc -c deck.c

run:
	./program

clean:
	rm *.o
	rm *~
