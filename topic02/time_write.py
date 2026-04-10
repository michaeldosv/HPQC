import sys
import time
import os

def main():
    # checks if there are the right number of arguments
    try:
        # converts the first two arguments to integers
        in_arg = int(sys.argv[1])
    except: # (argc != 2)
        # raises an error
        raise Exception("Incorrect arguments.\nUsage: python time_print.py [NUM]\ne.g.\npython time_print.py 23")

    # locate directory where current script is saved 
    script_loc = os.path.dirname(os.path.abspath(__file__))

    # Define the file path inside the 'data' folder
    file_path = os.path.join(script_loc, 'data', 'output.txt')

    start_time = time.time()

    with open(file_path, "w") as f:
        for i in range(in_arg):
            f.write("{}, ".format(i))

    end_time = time.time()

    run_time = end_time - start_time
    
    # Prints success message and output file save location
    print(f"File saved to: {file_path}")
    # prints the output
    print("\n\nTime for loop: {} seconds\n".format(run_time))

if __name__ == "__main__":
    main()



