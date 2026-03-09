import time
import os

def main():
	# locate file from python write script in the data folder
	file_path = os.path.join('data', 'output.txt')

	# error message in case the file does not exist
	if not os.path.exists(file_path):
		print('File not found. Run the write script first!')
		return

	# capture start time
	start_time = time.time()

	# open and read entire contents of the file
	with open(file_path, "r") as f:
		file = f.read()

	# capture end time
	end_time = time.time()

	# calculate time to open and read contents of file
	run_time = end_time - start_time

	# print to terminal contents runtime
	print('Time to read file: ', run_time, ' seconds')

if __name__ == "__main__":
	main()



