FLAGS = -w -pthread

all: producer_consumer reader_writer_1 reader_writer_2	reader_writer_3

clean:
	rm -f producer_consumer reader_writer_1 reader_writer_2 reader_writer_3

producer_consumer: producer_consumer_n_n.c
	gcc -o producer_consumer producer_consumer_n_n.c $(FLAGS)

reader_writer_1: reader_writer_1.c
	gcc -o reader_writer_1 reader_writer_1.c $(FLAGS)
	
reader_writer_2: reader_writer_2.c
	gcc -o reader_writer_2 reader_writer_2.c $(FLAGS)
	
reader_writer_3: reader_writer_3.c
	gcc -o reader_writer_3 reader_writer_3.c $(FLAGS)

