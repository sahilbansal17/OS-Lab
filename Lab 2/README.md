## Lab Assignment 2

### Part 1: FORK
- Write a program to generate a tree of processes through usage of fork().
- Each process can have atmost 2 children(fork() can be executed atmost twice).
- Let us consider that depth of this tree cannot exceed 4.
- After creation of the processes, each process determines number of descendants.
- One possible way is that each process runs "ps" command and tries to search for its id and checks how many processes ppid matches with its.
- Outcome: Each process prints its ID, parent ID, number of descendants and their IDs.

### Part 2: PIPE
- Four processes have to be created.
- First process reads data from a user specified file and through pipe sends this data to second process.
- Second process shall change case of every even character and pass this data to third process.
- Third process shall change case of every odd character and pass this data to fourth process.
- Fourth proces shall count number of words and pass this to the first process.

### Part 3: SHELL
Write code for a very simple shell that:
- prints out a prompt of your choice.
- reads user input (e.g ls, rm, mkdir). These commands can have their paramters.
- parses the line into the program name and an array of parameters (separated by delimiters).
- uses the fork() system call to spawn a new child process.
- The child process then uses the exec() system call (or one of its variants), to launch the specified program.
- The parent process (the shell) uses the wait() system call (or one of its variants), to wait for the child to terminate.
- Once the child (the launched program) finishes, the shell repeats the loop by jumping to the first step.
- When user types exit, the shell terminates.