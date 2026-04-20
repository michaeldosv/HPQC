# Topic 4: MPI Communications

## PART 1

For the first stage of demonstrating communications using MPI, the programme comm_test_mpi.c was run as is using a different number of processors 1 and 4.

Using 1 processor, the programme gives out an error that it is unable to communicate with less than 2 processors hence the code does not run.

Using 4 processors we get an output to the console that looks like this...

    Hello, I am 2 of 4. Sent 20 to Rank 0
    Hello, I am 3 of 4. Sent 30 to Rank 0
    Hello, I am 0 of 4. Received 10 from Rank 1
    Hello, I am 0 of 4. Received 20 from Rank 2
    Hello, I am 0 of 4. Received 30 from Rank 3
    Hello, I am 1 of 4. Sent 10 to Rank 0

When we examine the code we see that the programme splits the task into 4 processes each assigned ranks 0, 1, 2 and 3. Ranks 1, 2 and 3 are assigned as clients which carry out a calculation of the rank number * 10 and send their message to rank 0 which is the root which collects inputs from the other ranks. Rank 0 uses a for loop to wait for a response from rank 1 even if the other ranks are done before it. This is clearly evidenced by the output above where we see the received messages are printed in order. Meanwhile the send messages are out of order because ranks 1, 2 and 3 run independently and hence appear in random order when the code is rerun. This was confirmed to be the case.

The programme was then improved by altering its structure to be more readable and usable. The key changes were as follows...

- Separating functions : The function main() has been divided similarly to proof.c, check_uni_size(), delegate_task(), root_task() and client_task() to break down the original main() loop.
- Localised variables : Variables such as send_messgae and recv_message have been moved from global variables down to the function level where they are needed.
- Error messages : The function check_uni_size() makes sure the programme is run with >= 1 processors. If this is not the case, the programme returns an error message to the user.

Next, four different MPI send types were tried to assess how the programme functions with them instead of the standard MPI_Send. This involved a simple substitution of MPI_Send in the client_task() function. 

    No protocol specified
    Hello, I am 3 of 4. Sent 30 to Rank 0
    Hello, I am 0 of 4. Received 10 from Rank 1
    Hello, I am 0 of 4. Received 20 from Rank 2
    Hello, I am 0 of 4. Received 30 from Rank 3
    Hello, I am 1 of 4. Sent 10 to Rank 0
    Hello, I am 2 of 4. Sent 20 to Rank 0

**MPI_Ssend() :** Runs without any changes to the output.

**MPI_Bsend() :** Runs without any changes to the output.

**MPI_Rsend() :** Runs without any changes to the output.

**MPI_Isend() :** Programme does not compile due to a missing argument within the programme. 

It can be concluded that any MPI_Send() or any of the first three send types are compatible with the programme as it is.

When benchmarked using 4 processors, the result of the internal benchmark was as follows...

    Hello, I am 2 of 4. Sent 20 to Rank 0
    [RANK 2] took 0.000014 s to send
    Hello, I am 0 of 4. Received 10 from Rank 1
    [RANK 0] took 0.000050 s to recv from Rank 1
    Hello, I am 0 of 4. Received 20 from Rank 2
    [RANK 0] took 0.000001 s to recv from Rank 2
    Hello, I am 0 of 4. Received 30 from Rank 3
    [RANK 0] took 0.000011 s to recv from Rank 3
    Hello, I am 1 of 4. Sent 10 to Rank 0
    [RANK 1] took 0.000030 s to send
    Hello, I am 3 of 4. Sent 30 to Rank 0
    [RANK 3] took 0.000008 s to send

From the above results, the time taken to send and receive were inconsistent at best. This is in part due to the loop rank 0 is engaged in which waits until it receives communication from rank 1 even if ranks 2 or 3 are ready before rank 1. So the longer times for some of the ranks can be attributed to this process. The very small times frames can be attributed to the operating system which struggles to time the tasks. 

## PART 2

To measure latency and bandwidth in MPI communications, pingpong.c was written to bounce a communication between two processors multiple times defined by the user input. 

The programme was used to measure the latency at a variety of ping counts. The results are as follows...

*Target Pings : 10*

    Final Counter Value : 10
    Internal Benchmark : 0.000030 seconds
    Average Message Latency : 0.000001 seconds

*Target Pings : 1000*

    Final Counter Value : 1000
    Internal Benchmark : 0.001008 seconds
    Average Message Latency : 0.000001 seconds

*Target Pings : 100000*

    Final Counter Value : 100000
    Internal Benchmark : 0.068890 seconds
    Average Message Latency : 0.000000 seconds

*Target Pings : 1000000*

    Final Counter Value : 1000000
    Internal Benchmark : 0.581426 seconds
    Average Message Latency : 0.000000 seconds

Across all of the results we see that the average message latency starts at a slightly higher value but converges at practically zero as it takes so little time between messages as the number of pings reaches 1,000,000. The internal benchmark for the programme times increases which is to be expected as the amount of pings increases.

To measure the bandwidth of the MPI communications, the code pingpong.c was modified to take an additional input which would determine the size of the array to be transmitted between the root and client nodes (rank 0 and rank 1 respectively). This size is defined by the user on running the compiled code and is in bytes. A range of values were tried with a ping number of 10000 as a constant and using two processors. The results are as follows...

*8 Bytes*

    Target Pings : 10000
    Array Size : 8 Bytes (2 integers)
    Final Counter Value : 10000
    Average Transfer Time : 0.000000 seconds

*1 Kilobyte*

    Target Pings : 10000
    Array Size : 1024 Bytes (256 integers)
    Final Counter Value : 10000
    Average Transfer Time : 0.000001 seconds

*1 Megabyte*

    Target Pings : 10000
    Array Size : 1048576 Bytes (262144 integers)
    Final Counter Value : 10000
    Average Transfer Time : 0.000129 seconds

*2 Megabytes*

    Target Pings : 10000
    Array Size : 2097152 Bytes (524288 integers)
    Final Counter Value : 10000
    Average Transfer Time : 0.000255 seconds

By plotting the results and fitting a linear line to the data it is possible to extract some useful results and gain an understanding to how the latency and bandwidth are affected by increasingly larger array sizes.

Using Excel by plotting the average transfer time vs the size in bytes on a graph and adding a trendline, it was possible to determine the values m and c as per the equation of a linear line y = mx + c. The values were found to be m = 1e-10 and c = 7e-7. Interpreting the results in terms of the programme, m is the slope on the plot which is transfer time in seconds over the size of the array in bytes. The slope on the graph is essentially the time in seconds per byte. To convert this to bytes per second, we take the reciprocal of m, 1/m. This gives us a network bandwidth of ~ 10^10 bytes per second or 10 gigabytes per second. 

Meanwhile, c the y-intercept is the time per transfer for 0 bytes, meaning this is our network latency when no data is running between processors. 

The results for the linear fit are shown in bandwidth-determination.png. 

## PART 3

This next section looks at collective communications in MPI. Using the basis of the vector_mpi.c programme written for topic 3, some modified versions were implemented using MPI_Broadcaast(), MPI_Scatter() and MPI_Send() and MPI_Recv() as the "DIY" version. This was to test how these operations would work to simplify the work done previously in vector_mpi.c, specifically the creation and initialization of the vector array which was done by each processor. To reduce the computational workload, these alternative approaches offer a way to allow only the root processor to create the vector array and transmit it to the other processors.  

The MPI_Broadcast() approach sends an identical copy of the vector array to each processor. For this reason it is expected to be slowest as it has to send the largest piece of information to each processor of all the approaches.

MPI_Scatter() is expected to be the best approach as it divides the vector array into the amount of chunks that there are processors. It then sends each chunk to each processor simultaneously. Hence, it is expected that this approach will be the fastest and most efficient. 

The "DIY" approach, using MPI_Send() and MPI_Recv() makes used of a for loop for the root process which divides up the vector array based on the number of processors like MPI_Scatter() but sends each chunk sequentially to each processor using the for loop. Hence this is expected to by slightly slower than the MPI_Scatter() approach.

The results using 4 processors are as follows...

*MPI_Broadcast()*
*Vector array: 10*
BCAST Sum: 28 | Time: 0.000044 seconds
*Vector array: 100*
BCAST Sum: 4950 | Time: 0.000050 seconds
*Vector array: 100000*
BCAST Sum: 704982704 | Time: 0.001626 seconds

*MPI_Scatter()*
*Vector array: 10*
SCATTER Sum: 28 | Time: 0.000014 seconds
*Vector array: 100*
SCATTER Sum: 4950 | Time: 0.000047 seconds
*Vector array: 100000*
SCATTER Sum: 704982704 | Time: 0.000589 seconds

*MPI_Send() and MPI_Recv()*
*Vector array: 10*
DIY Sum: 28 | Time: 0.000035 seconds
*Vector array: 100*
DIY Sum: 4950 | Time: 0.000029 seconds
*Vector array: 100000*
DIY Sum: 704982704 | Time: 0.000653 seconds

From the results above we see that the MPI_Scatter() approach was fastest for the smallest and largest processes. The largest processes benefit greatly from the segmentation of the vector array and sending the chunks simultaneously. The MPI_Broadcast() approach shows that this approach is noticeably slower for a task like this with a result about 10x slower than the other two. It also appears the disparity between the results for the "DIY" approach and the MPI_Scatter() appraoches were fairly comparable for all test cases. This indicates the segmentation of the vector array played the biggest role in the speedup. and not the sending of the chunks simultaneously to the other processors. The results for the middle test case of 100 showed little to no change.  

Next, we test some variations of the MPI_Send() and MPI_Recv() processes by substituting them with some similar functions that work slightly differently. These are MPI_Gather() and MPI_Reduce(). 

**MPI_Send() and MPI_Recv() :** It is anticipated that this will be the slowest approach as the root process handles the return inputs from the client processes sequentially starting with the rank 1 process regardless of whether the other ranks are finished before it. This means that there is potential for long delays if all other processes return their results to the root process before rank 1.

**MPI_Gather() :** Is anticapted to be better than the above approach as the root process can gather the results of the other processes simultaneously. However, the root process still has to sequentially add all of those results together to get the final sum of the vector array. 

**MPI_Reduce() :** This is expected to be the fastest is it can allow the root process to collect a final sum of the other chunks. This is because intermediate processes handle the summation of vector array chunks, so only the final sum arrives at the root process. 

To test the programs vector_mpi.c , vector_mpi_gather() and vector_mpi_reduce(), the number of processors was kept constant at 4. Results are as follows...

*MPI_Send() and MPI_Recv()*
*Vector array: 10*
Sum: 28
Internal Benchmark : 0.000028
*Vector array: 1000*
Sum: 499500
Internal Benchmark : 0.000018 seconds
*Vector array: 100000*
Sum: 704982704
Internal Benchmark : 0.000202

*MPI_Gather()*
*Vector array: 10*
MPI_Gather Sum: 28
Time : 0.000008 seconds
*Vector array: 1000*
MPI_Gather Sum: 499500
Time : 0.000038 seconds
*Vector array: 100000)
MPI_Gather Sum: 704982704
Time : 0.000196 seconds

*MPI_Reduce()*
*Vector array: 10*
Sum: 28
Internal Benchmark : 0.000026 
*Vector array: 1000*
Sum: 499500
Internal Benchmark : 0.000022
*Vector array: 100000*
Sum: 704982704
Internal Benchmark : 0.000247

The results above show some inconsistent behaviour across the three approaches. MPI_Gather() seems to produce the best results at the highest vector array size of 100,000 meanwhile MPI_Reduce() is the slowest. However all results are on the some order of magnitude and are fairly comparable. MPI_Gather() stood out at the smallest vector array size of 10 which indicates that due to the small number of elements in the vector that summation of chunks before they reach the root processes does not appear to have an effect. 

Finally, a custom sum operation was implemented into a copy of the standard vector_mpi.c. This was done by creating a user defined function to carry out a sum operation on the vector array using the MPI_Reduce() approach. This programme vector_mpi_reduce_custom.c was run using the same inputs as the above tests. The results are shown below.

*Custom Sum operation using MPI_Reduce()*
*Vector array: 10*
Sum: 28
Internal Benchmark : 0.000007
*Vector array: 1000*
Sum: 499500
Internal Benchmark : 0.000055
*Vector array: 100000*
Sum: 704982704
Internal Benchmark : 0.000187 

From the above we see that the results are very similar to the timings seen for the MPI_Gather() approach in the previous step. Interestingly there is no noticeable speedup using the custom sum and MPI_Reduce() which theoretically should outperform MPI_Gather() due to the summation of vector array chunks being carried out more efficiently. 
