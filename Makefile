build:
	gcc -pedantic -Wall -std=c99 ./src/*.c -o renderer -I ./src/include -L ./src/lib -lmingw32 -lSDL2main -lSDL2

run:
	./renderer

clean:
	rm renderer
