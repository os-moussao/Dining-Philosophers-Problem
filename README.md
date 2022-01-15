## Understanding Threads and Processes
### Process:
A process is simply a program in execution
### Thread:
A thread is the unit of execution within a process. A process can have one or many threads (all executing a the same time).
### Process VS Thread:
- Time to terminate:\
	A process takes more time to terminate\
	A thread takes less time
- Resources:\
	A process consumes more resources \
	A thread consumes few resources
- Treatment by OS:\
	Different processes are treated separately by OS\
	All peer threads are treated as a single task by OS
- Memory & Sharing:\
	Treads share memory and share data with each other\
	Processes are mostly isolated

### Race conditions:
A race condition is when different threads access the same shared data, and they try to change it at the same time. In this case, these thread are "racing" for accessing and changing the data.\
In order to prevent race conditions from happening, you would have to put a "lock" around shared data to ensure that no other thread would try to access it while some thread is working on it.\
see: https://stackoverflow.com/a/34550/15175186 \
In C, to implement "a lock" around some data you will use pthread_mutex functions: [example](/Processes%20%26%20Threads/race_conditions_and_mutex.c).