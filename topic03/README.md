# Topic 3: Introduction to MPI 

## PART 1

The program hello_mpi.c was compiled and run by setting the number of processes to 4. The program ran without errors and gave the standard output.

To examine what would happen if the fundamental rules of MPI were broken, the line of programme where MPI_Init() was called was commented out. The programme was recompiled and run again. This resulted in an error message that the MPI command MPI_Comm_rank was called before MPI_Init which is disallowed by the MPI standard. The following line stated that the task was aborted because MPI was not able to aggregate error messages and was not able to guarantee that all other processes were aborted. Clearly this shows that any programme incorrectly written or compiled by MPI standards will fail to run because MPI cannot begin the task segmentation process nor allow separate nodes to communicate with each other.

To observe the runtime of hello_mpi.c using a different number of processes, the programme was altered, recompiled and rerun for 4, 8 and 16 processes. The time results are as follows.

*4 processes*
  real    0m0.448s
  user    0m0.132s
  sys     0m0.166s

*8 processes*
  real    0m0.466s
  user    0m0.228s
  sys     0m0.273s

*16 processes*
  real    0m0.473s
  user    0m0.357s
  sys     0m0.492s

Something that is evident here is that the sys + user time is greater than the real time. This is because the programme is spread across multiple cores working simultaneously hence the sys + user time is the sum of the time taken by each other cores individually. Meanwhile the real time only times the wall-clock time that it takes for the whole process to occur.

A serial variant of hello_mpi.c was written to compare the parallelized version to a serial version of the same programme. The results are as follows for the same numbers of processes tried.

*(serial) 4 processes*
  real    0m0.007s
  user    0m0.000s
  sys     0m0.005s

*(serial) 8 processes*
  real    0m0.005s
  user    0m0.004s
  sys     0m0.000s

*(serial) 16 processes*
  real    0m0.005s
  user    0m0.004s
  sys     0m0.000s

Comparing the above results to the parallelized hello_mpi.c, the real time in the serial programmes is much closer to the sum of the sys + user time. This is to be anticipated as the serial process runs on one core and handles the tasks sequentially. The parallelized programmes also take noticeably longer to run than the serial versions indicating this type of task does not benefit from parallelization as it takes longer to segment the task into chunks than to just handle it sequentially. 

## PART 2

The main functions of proof.c were identified and are described below.

**main() :** Defines the structure of the program. Initializes MPI and finalizes MPI. Interprets and divides the command line input to identify the multiplier argument. From the command line argument it identifies the 

**check_args() :** Checks the user input to ensure exactly two arguments were provided. If the input is correctly provided, it converts the input string to integer and returns it. Otherwise it displays an error message to the user.

**check_uni_size() :** Checks the user input to ensure the number of processes requested is >= 1. If this is true, the function returns the number of processes. If not, the function displays an error message to the user.

**check_task() :** Reroutes the process based on MPI rank. If the process has a rank of 0 it is passed to root_task(). All other processes are passed to client_rank().

**client_task() :** Calculates a value by multiplying the MPI rank of a process by the user defined num_arg. It then uses MPI_send to send the process to rank 0.

**root_task() :** Receives the results of other processes using MPI receive and sums the results all together. Returns the output of the sum.

A flowchart of proof.c was drawn and is available as proof.flow.png in the topic03.

The function root_task() is responsible for summing the results of the other processes. This can be represented as

$Total = \sum_{i=1}^{N-1} /left( i A /right)

Where N is the uni_size, A is the num_arg both defined by the user. A remains constant throughout the summation so it can be factored out. 

$Total = A \sum_{i=1}^{N-1} /left( i /right)

This is the formula behind what the programme is doing. 

## PART 3

The key functions of vector_serial.c are documented below.

**main() :** Checks that the correct number of arguments are given by the user using chech_args(). Allocates an appropriate amount of computer memoory to store vector. Uses those inputs to create a vector using initialise_vector(). Sums the components of the created vector using sum_vector(). Prints the outcome of this sum using print_vector(). Frees up memory used to store vector.

**sum_vector() :** Creates a variable sum equal to 0. Sums the contents of the vector passed as an argument. Returns the sum of all vector components as an integer.

**initialise_vector() :** Creates a vector with a size given by the user arguments. It does this using a for loop to populate the vector array.

**print_vector() :** Prints the vector to the terminal in terms of its components.

**check_args() :** Checks that only two arguments are given by the user. If two arguments are given, the function returns these values as integers. If not then an error is given to the user.

The programme vector_serial was modified to create a non-trivial vector by introducing a modification to the code that filled the vector with the index number if the iterative list. The programme was then benchmarked internally and externally using a range of values. 

The serial programme was benchmarked internally and externally using different inputs. The results include the sum of the vector, internal benchmark and external benchmarks are as follows.

*Input : 10*
  Sum: 45
  Internal Benchmark : 0.000003 seconds
  real    0m0.004s
  user    0m0.000s
  sys     0m0.004s

*Input : 10000*
  Sum: 49995000
  Internal Benchmark : 0.000101 seconds
  real    0m0.007s
  user    0m0.004s
  sys     0m0.001s

*Input : 65000*
  Sum: 2112467500
  Internal Benchmark : 0.000460 seconds
  real    0m0.005s
  user    0m0.005s
  sys     0m0.000s

*Input : 100000*
Sum: 704982704
Internal Benchmark : 0.000708 seconds
real    0m0.006s
user    0m0.006s
sys     0m0.000s

A parallel version of the vector_serial.c programme was written using MPI to divide the vector into local chunks which would be processes on separate cores. This was done to create a point of comparison between the serial version and parallelized version.

The parallel version of the programme was run using the same range of inputs as the serial for comparison.

*Input : 10*
  Sum: 45
  Internal Benchmark : 0.000003 seconds
  real    0m0.421s
  user    0m0.018s
  sys     0m0.044s

*Input : 10000*
  Sum: 49995000
  Internal Benchmark : 0.000062 seconds
  real    0m0.369s
  user    0m0.025s
  sys     0m0.032s

*Input : 65000*
  Sum: 2112467500
  Internal Benchmark : 0.000207 seconds
  real    0m0.424s
  user    0m0.033s
  sys     0m0.025s

*Input : 100000*
  Sum: 704982704
  Internal Benchmark : 0.000673 seconds
  real    0m0.426s
  user    0m0.026s
  sys     0m0.032s

The above results show a lot. What is immediately clear is that there is an overflow point beyond inputs of 100,000 or more which is evidenced by the fact the results of the sum are not accurate. This occurs due to the limit with integers in C which are 32 bit. In these cases the output sum of the vector components is larger than what 32 bit integers can display.
The internal benchmarks show that parallelization is successful at speeding up the programme for inputs 10,000 and 65,000. The speedup is not evident for input 10 as this is too small a value to quantify the benchmark time. 
The real time for all parallel runs is around 0.4 s regardless of the input. This is interesting as it clearly shows the overhead required by MPI to segment the task for parallelization.
