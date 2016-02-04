all:
	gcc -Wall -o dense_mm dense_mm.c
	gcc -Wall -o do_malloc do_malloc.c
	gcc -Wall -o do_nothing do_nothing.c
	gcc -Wall -o parallel_dense_mm parallel_dense_mm.c -fopenmp
	gcc -Wall -o sing sing.c

clean:
	rm -f *.o
	rm dense_mm
	rm do_malloc
	rm do_nothing
	rm parallel_dense_mm
	rm sing
