sock : test-client.o struct.o
	gcc -o sock test-client.o struct.o -g -lwebsockets -lm

test-client.o : test-client.c client.h package.h
	gcc -o test-client.o -c test-client.c -Wall -g -lm

struct.o : struct.c struct.h
	gcc -o struct.o -c struct.c -Wall -g -lm

clean:
	rm -rf *.o
