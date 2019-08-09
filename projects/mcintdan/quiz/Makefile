CFLAGS= -coverage

testme:
	gcc -o testme testme.c -g $(CFLAGS)
	testme > testmeresults.out
	gcov testme.c -b >> testmeresults.out
	gcov testme.c -f >> testmeresults.out
	cat testme.c.gcov >> testmeresults.out	

clean:
	rm -f *.o *.gcov *.gcda *.gcno testme