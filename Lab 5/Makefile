all: resource_allocate reader_writer

resource_allocate: resource_allocation.cpp
	g++ -o resource_allocate resource_allocation.cpp

reader_writer: Reader\ Writer/solution_1.c
	gcc -o solution_1 Reader\ Writer/solution_1.c -lpthread

clean:
	rm -f resource_allocate
	rm -f solution_1
