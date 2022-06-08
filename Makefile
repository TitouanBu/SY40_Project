all : prog

prog : main.o CodingFunctions.o
	gcc -o prog main.o CodingFunctions.o -pthread

main.o : main.c
	gcc -o main.o -c main.c -pthread

CodingFunctions.o : CodingFunctions.c
	gcc -o CodingFunctions.o -c CodingFunctions.c -pthread

clean : 
	rm -rf *.o

mrproper : clean
	rm -rf prog