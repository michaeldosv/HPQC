#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double to_second_float(struct timespec in_time);
struct timespec calculate_runtime(struct timespec start_time, struct timespec end_time);

int main()
{
	struct timespec start_time, end_time, time_diff;
	double runtime = 0.0;

	// initialise variables and pointers
	FILE *file_pointer;
	char *buffer;
	long file_size;

	// assign text file to variable
	file_pointer = fopen("data/output_c.txt", "r");

	// error message if file cannot be found
	if (file_pointer == NULL)
	{
		fprintf(stderr, "Error: Could not open file. Run the write script first");
		exit(-1);
	}
	
	// start timing
	timespec_get(&start_time, TIME_UTC);

	// find out how big the file is (in bytes)
    	fseek(file_pointer, 0L, SEEK_END); // Fast-forward to the end of the file
    	file_size = ftell(file_pointer);   // Read the byte position (which gives us the size)
    	rewind(file_pointer);              // Rewind back to the start of the file

    	// allocate exact memory needed
    	buffer = (char *)malloc(file_size + 1);
    	if (buffer == NULL) 
    	{
        	fprintf(stderr, "Error: Memory allocation failed\n");
        	fclose(file_pointer);
        	exit(-1);
    	}

    	// read the entire file into the memory buffer
    	fread(buffer, file_size, 1, file_pointer);
    	buffer[file_size] = '\0'; // cap off the string

    	// stop timing
    	timespec_get(&end_time, TIME_UTC);

    	// close file 
    	fclose(file_pointer);
    
	// calculate runtime
    	time_diff = calculate_runtime(start_time, end_time);
    	runtime = to_second_float(time_diff);

	// print runtime
	printf("Runtime for reading file: %lf seconds.\n\n", runtime);

	// free up memory 
	free(buffer);

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
	long int seconds, nanoseconds;
	double runtime = 0.0;

	// extracts the elements from start_time and end_time
	seconds = end_time.tv_sec - start_time.tv_sec;
	nanoseconds = end_time.tv_nsec - start_time.tv_nsec;

	// if the ns part is negative
	if (nanoseconds < 0)
	{
		//  "carry the one!"
		seconds = seconds - 1;
		nanoseconds = ((long int) 1e9) - nanoseconds;
	}

	// creates the runtime
	time_diff.tv_sec = seconds;
	time_diff.tv_nsec = nanoseconds;

	return time_diff;
}
