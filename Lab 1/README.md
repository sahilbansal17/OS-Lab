## Lab Assignment 1

### Part 1: Implement Linux Command "tree"

Write a C program to implement "tree" command, which is used to display the subdirectories and files within the current directory (where command is run). Directory structure is traversed in DFS manner and displayed as such. An example is shown below.

Your program should accept a command line argument. If the argument is -b, the tree structure should be displayed in BFS (breadth first search) manner and if argument is -d, the structure should be displayed in DFS (depth first search) manner.

```
etc
|-- abrt
|   |-- abrt-action-save-package-data.conf
|   |-- abrt.conf
|   |-- gpg_keys
|   `-- plugins
|       |-- CCpp.conf
|       `-- python.conf
|-- acpi
|   |-- actions
|   |   `-- power.sh
|   `-- events
|       |-- power.conf
|       `-- video.conf
|-- adjtime
|-- aliases
|-- aliases.db
|-- alsa
|   `-- alsactl.conf
```

### Part 2: CPU Scheduling Simulation
Write a C program (preferably for linux gcc compiler) to simulate CPU scheduling.

Following CPU scheduling mechanisms need to be implemented:
- FCFS
- SJF
- Priority
- MLFQ
- Round Robin
- Lottery (proportional share)

The process traces should be read from a file called "process.dat". Format for this file is as follows:
```
<number of processes>
<pid>  <arrival time>   <priority>  <share> C <burst>  I/O <burst>  C <burst> .......  -1
....
<pid>  <arrival time>   <priority>  <share> C <burst>  I/O <burst>  C <burst> .......  -1
```

So first line describes number of processes (say N).

Each of next N lines describe process trace. <pid> is process id. <arrival time> is the time this process is put into ready queue. Here C stands for CPU, I stands for Input device and O stands for output device.  -1 indicates the end of trace for a process. <priority> is the priority assigned to a process. A lower value means higher priority. <share> means % of tickets assigned in lottery (proportional share) scheduling.

We are assuming one CPU, one Input device and one output device. Input/output devices follow first come first served policy i.e. once a process is assigned input/output device, this device can not be assigned to any other process till its burst is over.

For Round Robin scheduling, quantum shall be one.

For MLFQ scheduling, we shall assume that there are three queues - high, medium and low priority. Also process priority (except for those in high priority queue) is incremented every 100 cycles.

For priority queue, both preemptive and non-preemptive versions need to be implemented.

In lottery scheduling, total number of tickets are 100. So share of all processes should sum to 100.