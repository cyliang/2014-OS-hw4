MT: read_write.o main.o
	gcc -o MT -lpthread read_write.o main.o

.PHONY: verbose
verbose: VERBOSE = -DVERBOSE
verbose: MT

single: single.o read_write.o
	gcc -o single single.o read_write.o

rand: rand.c
	gcc -o rand rand.c

read_write.o: header.h read_write.c
	gcc -c -o read_write.o read_write.c

single.o: header.h single.c
	gcc -c -o single.o single.c

main.o: header.h main.c
	gcc -c $(VERBOSE) -o main.o main.c

.PHONY: clean
clean:
	@rm -f read_write.o single.o main.o single rand output.txt MT
	@echo "Cleaned"
