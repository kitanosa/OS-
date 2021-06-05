all: static dynamic 
	rm -f *.o

static: static.o libimp0.so
	gcc -o static static.o -L. -limp0 -lm -Wl,-rpath,.

static.o: ./src/static.c
	gcc -c ./src/static.c -lm

libimp0.so: sinintegral.o e.o
	gcc -shared -o libimp0.so sinintegral.o e.o -lm

sinintegral.o: ./src/lib/imp_0/src/sinintegral.c
	gcc -c -fPIC ./src/lib/imp_0/src/sinintegral.c -lm

e.o: ./src/lib/imp_0/src/e.c
	gcc -c -fPIC ./src/lib/imp_0/src/e.c -lm

dynamic: dynamic.o libimp0.so libimp1.so
	gcc -o dynamic dynamic.o -L. -limp0 -limp1 -lm -ldl -Wl,-rpath,.

dynamic.o: ./src/dynamic.c
	gcc -c ./src/dynamic.c -lm

libimp1.so: sinintegral1.o e1.o
	gcc -shared -o libimp1.so sinintegral1.o e1.o -lm

sinintegral1.o: ./src/lib/imp_1/src/sinintegral.c
	gcc -c -fPIC ./src/lib/imp_1/src/sinintegral.c -o sinintegral1.o -lm

e1.o: ./src/lib/imp_1/src/e.c
	gcc -c -fPIC ./src/lib/imp_1/src/e.c -o e1.o -lm

clean:
	rm -f *.o *.so static dynamic