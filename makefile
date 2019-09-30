make: comp fin clean
	

clean:
	rm -rf *.o	

fin:
	g++ level.o cannon.o main.o -o cannon.out -lsfml-graphics -lsfml-window -lsfml-system

comp:
	g++ -c -g level.cpp cannon.cpp main.cpp

run: make
	./cannon.out

debug: make
	valgrind ./cannon
