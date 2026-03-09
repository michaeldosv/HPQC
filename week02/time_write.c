#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double to_second_float(struct timespec in_time);
struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time);

int main(int argc, char **argv) 
{
	// creates and initialises the variables
	int i, input;
	i = input = 0;
	struct timespec start_time, end_time, time_diff;
	double runtime = 0.0;
	
	// * file initialised as pointer
	FILE *file_pointer;
	
	// checks if there are the right number of arguments
	if (argc == 2)
	{
		// converts the first argument to an integer
		input = atoi(argv[1]);
	}
	else //(argc != 2)
	{
		// raises an error
		fprintf(stderr, "Incorrect arguments.  Usage: time_print [NUM]\ne.g. \n time_print 3\n");
		// and crashes out
		exit(-1);
	}
	
	// * write to new output file in the data folder
	// NOTE data folder MUST already exist for this to function correctly
	file_pointer = fopen("data/output_c.txt", "w");

	if (file_pointer == NULL)
	{
		fprintf(stderr, "Error: Could not open file");
		exit(-1);
	}

	// gets the time before the loop
	timespec_get(&start_time, TIME_UTC);
	// iterates over all numbers up the input
	for (i = 0; i < input; i++)
	{
		// * writes the index to the file
		fprintf(file_pointer, "%d, ", i);
	}
	// gets the time after the loop
        timespec_get(&end_time, TIME_UTC);

	// * close the file pointer to ensure data is saved
	fclose(file_pointer);

	// calculates the runtime
	time_diff = calculate_runtime(start_time, end_time);
	runtime = to_second_float(time_diff);


	// outputs the runtime
	printf("\n\nRuntime for core loop: %lf seconds.\n\n", runtime);

	return 0;
}


double to_second_float(struct timespec in_time)
{
	// creates and initialises the variables
	float out_time = 0.0;
	long int seconds, nanoseconds;
	seconds = nanoseconds = 0;

	// extracts the elements from in_time
	seconds = in_time.tv_sec;
	nanoseconds = in_time.tv_nsec;

	// calculates the time in seconds by adding the seconds and the nanoseconds divided by 1e9
	out_time = seconds + nanoseconds/1e9;

	// returns the time as a double
	return out_time;
}

struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time)
{
	// creates and initialises the variables
	struct timespec time_diff;
	long int seconds, nanoseconds;                                                                                                       seconds = nanoseconds = 0;
	double runtime = 0.0;

	// extracts the elements from start_time and end_time
	seconds = end_time.tv_sec - start_time.tv_sec;
	nanoseconds = end_time.tv_nsec - start_time.tv_nsec;

	// if the ns part is negative
	if (nanoseconds < 0)
	{
		// "carry the one!"
		seconds = seconds - 1;
		nanoseconds = ((long int) 1e9) - nanoseconds;
	}

	// creates the runtime
	time_diff.tv_sec = seconds;
	time_diff.tv_nsec = nanoseconds;

	return time_diff;
}

