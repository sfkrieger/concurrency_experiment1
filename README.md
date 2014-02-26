concurrency_experiment1
=======================

This is a C application that benchmarks the relationship between the number of threads and the access time to data stored in a shared queue.

Input to the program is the number of threads to create and runtime of the program. 
Output is the number of successful operations completed on the queue. 
Valid operations are simply enqueueing and deqeueing an arbitrary value.
