# Lab Assignment 4

### Producer Consumer Problem
- [One producer and One consumer](producer_consumer_1_1.c)
- [Multiple producers and Multiple consumers](producer_consumer_n_n.c)

### Reader Writer Problem
- [Readers-preference](reader_writer_1.c)
- [Writers-preference](reader_writer_2.c)
- [Fair solution](reader_writer_3.c)

### Running the programs
- A makefile has been provided.

	````$ make````

	This will compile all the code files and produce executables.

- #### Producer Consumer Problem

	````$ ./producer_consumer <no. of producers> <no. of consumers> <buffer size>````

- #### Reader Writer Problem
	- ##### Readers-Preference

		````$ ./reader_writer_1 <no. of writers> <no. of readers>````
	- ##### Writers-Preference

		````$ ./reader_writer_2 <no. of writers> <no. of readers>````
	- ##### Fair Solution

		````$ ./reader_writer_3 <no. of writers> <no. of readers>````

- #### Remove the executables
	````$ make clean````