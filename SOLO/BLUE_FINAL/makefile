sock : test-client.o struct.o
	gcc -o blue test-client.o struct.o -g -lwebsockets -lm

test-client.o : test-client.c client.h
	gcc -o test-client.o -c test-client.c -Wall -g -lm

struct.o : ../communs_final/struct.c ../communs_final/struct.h
	gcc -o struct.o -c ../communs_final/struct.c -Wall -g -lm

clean:
	rm -rf *.o
