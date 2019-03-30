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

### Dining Philosopher's problem

1. Each philosopher tries to acquire two chopsticks.
2. Each philosopher tries to acquire first left chopstick and then right chopstick (incorrect and leads to deadlock)
3. Restrict number of philosophers eating at any time to four.
4. Chopsticks are numbered. Each philosopher tries to acquire lower numbered chopstick first.
5. All odd numbered philosopher try to pick left chopstick and then right chopstick, and all even numbered try to pick right and then left chopstick.
6. Solution using arbitrator (philosopher needs to request an arbitrator for chopstick. It is for arbitrator to see that deadlock does not take place)
7. Chandy Misra solution

        $ make
        $ ./dining_philosopher <solution no.> <no. of philosophers>
