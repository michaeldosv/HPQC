# Topic 2: Performance & Parallelism

In topic 2 we are asked to time some simple tasks such as Linux commands and basic programs.

### Timing Basic Commands

Starting with running the `ls` and `cd` commands in Linux and timing the results using `time`. The results are as follows.

Result of `time ls`

	real    0m0.006s
	user    0m0.000s
	sys     0m0.005s

Another trivial command was timed which is was entering another folder within the directory. The command run in this case was to enter the week 1 folder within the coursework directory. 

Result of time `cd week01`

	real    0m0.001s
	user    0m0.000s
	sys     0m0.000s

From benchmarking the `ls` and `cd` commands it is clear the real time dominates. This is the start time subtracted from the end time of running the whole process. The user time which is the time spent by the computer to calculate and run the process on its core is effectively zero for a trivial process like this. The sys component is comparable to the real time which tells us the that the majority of the time spent to execute the command ls is spent system tasks.

### Externally Timing Basic Scripts

Next, a 'Hello world!' script was run using python and C and benchmarked using the time command. The results are as follows.

Result of `time python 3 hello_world.py`

	Hello, World!
	
	real    0m0.040s
	user    0m0.030s
	sys     0m0.008s

We see that the time to run the script is greater than running the trivial commands above. The real component continues to dominate. 

The C script needed to be compiled before use using `gcc`. Once a binary file was generated, the script could be run.

Result of time `./hello_world`

	Hello, World!
	
	real    0m0.007s
	user    0m0.000s
	sys     0m0.005s

Evidently, the C 'Hello world' script is much quicker to run once compiled. Python clearly takes much longer due to the compiler. However, the time above does not take into account the amount of time taken to compile the original C file into binary nor the time taken to write the commands into the terminal. Hence, we cannot draw many conclusions from the results above.

Next, simple multiplication was simulated using the repeat_adder programs in both Python and C. These programs would take two integers as inputs and perform repeated addition of integer 1 an x amount of times given by integer 2. The scripts were run using a variety of cases including extreme cases such as 2^32.

Some notable findings were in the differences in how Python and C handle data and how that impacts the speed of execution. For the inputs of 10^6 x 5x10^6, the real time for the Python script was on the order of 10 times greater than the C script which was dominated by the user component. The C script ran into errors when the input values were greater than 2^31 - 1 which resulted in the script outputting a value of 0. This is due to a overflow issue in C as standard integers in C are limited to 32 bits (2^31 - 1). Python was able to cope with an input of 2^31 - 1 because it dynamically allocates memory for integers. The Python script failed to run once the output reached values > 2^32.

### Internally Timing I/O

Finally, I/O in Python and C was investigated internally. This removes the need for numbers to be manually inputted into the terminal and allows for investigation of runtimes in Python and C to be more accurately examined. 

Writing integers up to 10^6 to a file in C was significantly faster than Python where the C script reported a writing time of 0.071s while the Python script reported a figure almost 5 times greater at 0.375s. Reading the contents of these files using a separate script in both languages showed very little difference between the two with the C script taking 0.072s and Python taking 0.080s to read the contents of the respective files generated previously. This is in comparison to printing integers up to 5x10^5 into the terminal for which the times were on comparable order of magnitude between Python (0.359s) and C (0.206s). 

### Conclusion

From timing various scripts in both Python and C it is clear that Python has significant overhead for the code interpreter in comparison to C which once compiled, does not have. Python has the advantage of automatic memory management which is useful for large integers to prevent overflow errors. Meanwhile, C requires the programmer to be more vigilant and understand what integer types will be present when handling the data but in turn is faster once correctly compiled at handling large data and I/O.


