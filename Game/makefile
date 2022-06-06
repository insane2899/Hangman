
Hangman:	WordPartitions.o WordList.o main.o 
	gcc -g -o Hangman main.o WordPartitions.o WordList.o

main.o: main.c 
	gcc -c main.c -o main.o

WordPartitions.o: WordPartitions.c 
	gcc -c WordPartitions.c -o WordPartitions.o

WordList.o:	WordList.c
	gcc -c WordList.c -o WordList.o


clean:
	rm *.o Hangman