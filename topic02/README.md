# Topic 2: Performance & Parallelism

In topic 2 we are asked to time some simple tasks such as Linux commands and basic programs.

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

From running the time ls command through the terminal it is clear the real time dominates which is the start time subtracted from the end time of running the whole process. The user time which is the time spent by the computer to calculate and run the process on its core is effectively zero for a trivial process like this. The sys component is comparable to the real time which tells us the that the majority of the time spent to execute the command ls is spent system tasks.
