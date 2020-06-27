Multi-user-Chat-System


A. USAGE
1. To compile:
	cd src/
	make all
2. Donot delete server file. Make sure it has atleast 2 lines written in it before you start (newline counts as a line).
3. To use:
	First create a server process: ./server_code
	Then create one or more client processes:	./client_code
4. Every client user can send a message to a specific user or to all users currently logged in, depending on his/her preference.



B.  COMMON FUNCTIONS IN server.c AND client.c
1. int AppendToFile(char* filename, char* sentence)
	appends a new line character, followed by a string to a file
2. char* read_nth_line(char* filename, int current_line_to_read)
	reads the nth line of a file
3. int getNumberofLinesInFile(char* filename)
	gets the number of lines in a given text file



C. DESCRIPTION OF server.c
0. Infinite while loop runs
1. Create an array of usernames
2. Read a line from server. Every line is of the format [SEND ALL], [SEND <USERNAME>] or [NEW USER]
3. Based on the type of request, process it.
	No threading is needed here



D. DESCRIPTION OF client.c
0. Inifnite while loop
1. Initialising - create file of <username>. Send a message to server that [NEW USER] <username>
2. Two threads run parallely
	thread1: Check if anything was written to user's file, print it
	thread2: write to server if user presses ENTER



E. ERROR CASES HANDLED
1. "Error! File does not exist" -> means file doesn't exist
2. If you try to make a new user with an existing username, server ignores your request
3. "ERROR You requested a line greater than what exists in the file" -> you request a line greater than what exists in a text file (internal working)
4. "Error. Cannot process undefined type of request" -> when we get a type of request not of type 'A'/'O'/'N'


This was an assignment in Computer Architecture & Operating Systems course at IIIT Delhi.
