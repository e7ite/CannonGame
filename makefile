make: comp
	g++ cannon.o main.o -o cannon -lsfml-graphics -lsfml-window -lsfml-system

comp:
	g++ -c cannon.cpp main.cpp

run: make
	./cannon
