sock : test-client.o motion.o parsing.o struct.o
	gcc -o sock test-client.o motion.o parsing.o struct.o -g -lwebsockets -lm

test-client.o : test-client.c client.h package.h
	gcc -o test-client.o -c test-client.c -Wall -g -lm

struct.o : struct.c struct.h
	gcc -o struct.o -c struct.c -Wall -g -lm

motion.o : motion.c motion.h
	gcc -o motion.o -c motion.c -Wall -g -lwebsockets

parsing.o : parsing.c parsing.h
	gcc -o parsing.o -c parsing.c -Wall -g

clean:
	rm -rf *.o
