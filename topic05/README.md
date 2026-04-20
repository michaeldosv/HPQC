# Topic 5: Communicators and Topologies

## PART 1

The first part ask to update the string_wave.c programme to remove hard coding and allow user defined user inputs through the command line. This is documented below. 

**Define "struct" container :** This stores all variables that will be user defined through the command line. 

**Rewrite check_args() function :** The check_args() function had to be rewritten to suite the new number of inputs given by the user. Firstly the function is no longer an int check_args() but a SimArgs check_args(). This was updated at the top of the programme when the function is initially defined. Check_args() also now checks that five arguments are passed when the programme is run, programme name, no. of points, no. of cycles, no. of samples and filepath for .csv file output. Finally the function was updated to record each of these user inputs as a variable using indexing. 

**Changes to main() :** These involves adding a reference to the user defined inputs as opposed to the hard coded values present previously.

Similarly, changes were made in the python programme, animate_line_file.py to remove hardcoding also. These changes are as follows.

**Get_filenames() updated :** This removing the existing indexing code for one file sys.argv[1] and replacing it with two variables input_filename and output_filename which use an indexing method same as the previous line, sys.argv[1] and sys.argv[2] respectively to capture the file to read data from (.csv) and filepath to output the (.gif) file to. Error messages for incorrect number of variables were also updated.

**main() changes :** A change was made at the very top of the function to capture two variables instead of one, input and output files. A change was made at the bottom to remove generate_path with the output_filename path defined by the user. 

## PART 2

For this part, the programme string_wave.c was modified from a serial version to a parallelized version using MPI. The new programme called string_wave_mpi.c was written with the approach that is outlined below.

*Parallelization strategy*

The parallelization strategy to take the vibrating string model and decompose into smaller lengths of string. This was done as in terms of the physics, the new position of the current point on the string depends on the position of its nearest left neighbouring point. In terms of the programming approach, the string was evenly divided among the number of processors allocated by the user when running the programme. Each processor only allocates memory for its section of the string.

To allow sections to update their position based on the left nearest neighbour as outlined above, a communication between neighbouring sections is required. In particular once the end of the string is reached. Before the communication phase begins between sections, each processor uses MPI_Send() to send its position to the rightward neighbour. At the same time, each right neighbour receives the communication using MPI_Recv(). The section of section of string at the edge of the string is driven by the simple harmonic oscillator model, which in turn drives the motion of the following sections. 

*Aggregations strategy*

The aggregations strategy chosen for this task was memory based. This meant that MPI_Gather() was used at the end of each computation step and all distributed sections on other processors would then be recombined once these processes were done, back to a single processor which would output the final state of the entire string. The root process of rank 0 is the only process that is responsible for allocating memory for the final array that combines the outcomes of all the other sections. It then outputs the final result to the csv file. This means that the final stage of the programme is serial in nature, however this was considered optimal to data being outputted to the final file in disorderly ways that results in the behaviour being shown on the final graph.

The results of the task are as follows:

**Access to HPC computer was interrupted before the remainder of Topic 5 could be completed** 


## PART 3

