build:
	gcc -std=c99 ./src/*.c -o renderer -I ./src/include -L ./src/lib -lmingw32 -lm -lSDL2main -lSDL2

run:
	./renderer

clean:
	rm renderer
