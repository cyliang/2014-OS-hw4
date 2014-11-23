MT: read_write.o main.o job.o
	gcc -o MT -lpthread read_write.o main.o job.o

rand: rand.c
	gcc -o rand rand.c

read_write.o: header.h read_write.c
	gcc -c -o read_write.o read_write.c

main.o: header.h job.h main.c
	gcc -c -o main.o main.c

job.o: job.c job.h
	gcc -c -o job.o job.c

.PHONY: clean
clean:
	@rm -f read_write.o main.o job.o rand output.txt MT
	@echo "Cleaned"
