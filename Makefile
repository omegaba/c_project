CC=gcc
CFLAGS= 	-Wall  -g  -pedantic -std=c11


test_unbounded: test_unbounded.c unbounded_int.c
	$(CC) $(CFLAGS) test_unbounded.c -o test_unbounded
calc_unbounded_int: calc_unbounded_int.c unbounded_int.c
	$(CC) $(CFLAGS) calc_unbounded_int.c -o calc_unbounded_int


clean :
	rm -rf *~

cleanall:
	rm -rf *~ *o  calc_unbounded

