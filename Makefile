CC = gcc

CFLAGS = -Wall -g

LDFLAGS = 

lorafd: main.o fdlibrary.a
	$(CC) $(CFLAGS) main.o fdlibrary.a -o lorafd $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

fdlibrary.a: fdlibrary.o
	ar rcs fdlibrary.a fdlibrary.o

fdlibrary.o: fdlibrary.c fdlibrary.h
	$(CC) $(CFLAGS) -c fdlibrary.c -o fdlibrary.o

clean:
	rm main.o fdlibrary.o fdlibrary.a lorafd
