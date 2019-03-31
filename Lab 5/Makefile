all: resource_allocate reader_writer dining_philosopher producer_consumer

resource_allocate: resource_allocation.cpp
	g++ -o resource_allocate resource_allocation.cpp

reader_writer: Reader\ Writer/solution_1.c
	gcc -o solution_1 Reader\ Writer/solution_1.c -lpthread

dining_philosopher: Dining\ Philosophers/solution.c
	gcc -w -o dining_philosopher Dining\ Philosophers/solution.c -lpthread

producer_consumer: Producer\ Consumer/producer_consumer.c
	gcc -w -o producer_consumer Producer\ Consumer/producer_consumer.c -lpthread

clean:
	rm -f resource_allocate
	rm -f solution_1
	rm -f dining_philosopher
	rm -f producer_consumer