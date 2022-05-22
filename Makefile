CC=gcc
CFLAGS=     -Wall  -g  -pedantic -std=c11  -o
PROGRAMS =  test_unbounded  calc_unbounded_int 

all :  $(PROGRAMS)


test_unbounded :  test_unbounded.o unbounded_int.o
	$(CC) $(CFLAGS) test_unbounded test_unbounded.o unbounded_int.o

calc_unbounded_int : calc_unbounded_int.o unbounded_int.o
	$(CC) $(CFLAGS) calc_unbounded_int calc_unbounded_int.o unbounded_int.o



test_unbounded.o : test_unbounded.c 
	$(CC) $(CFLAGS) test_unbounded.o test_unbounded.c -c

unbounded_int.o : unbounded_int.c
	$(CC) $(CFLAGS) unbounded_int.o unbounded_int.c -c

calc_unbounded_int.o : calc_unbounded_int.c
	$(CC) $(CFLAGS) calc_unbounded_int.o calc_unbounded_int.c -c



clean :
	rm -rf *~

cleanall:
	rm -rf *~ *o  $(PROGRAMS)