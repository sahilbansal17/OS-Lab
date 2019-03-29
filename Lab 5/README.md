## Lab Assignment 5

### Banker's Algorithm
This algorithm is used to identify whether a given initial resource allocation to processes is safe or unsafe. 

If it is safe, then any one possible process execution sequence is printed. 

- To run the program: 

		$ make
		$ ./resource_allocate < input file >

Some sample input files have been provided in the [testCases](testCases/) folder.

- To remove the executables formed:

		$ make clean

### Reader Writer's problem
Solution using shared memory

1. Reader's preference solution

		$ make
		$ ./solution_1 < no. of writers > < no. of readers >