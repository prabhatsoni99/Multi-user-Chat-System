#include <stdio.h>
#include <stdlib.h>  // For exit() function
#include <string.h>
#include <pthread.h> // for threading

/*
Line is written as:

<data> + "\n"
Thus, 1st line of text file will always be empty, and useless
*/

#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0]))
// gets number of elements in an array


////////////////////////////////////////////////////////////////////////////
/////////////////	COMMON FUNCS BELOW	////////////////////////////////////
////////////////////////////////////////////////////////////////////////////


int AppendToFile(char* filename, char* sentence)
{
	FILE *fptr;
	fptr = fopen(filename, "a"); // a is for append
	if(fptr == NULL)
	{
		printf("Error! File does not exist");
		exit(1);
	}

	char sentence_charArray[1000];
	strcpy(sentence_charArray, sentence); // converting string pointer -> character array

	char writeable_string[] = "\n";
	strcat(writeable_string, sentence_charArray);
	fprintf(fptr,"%s", writeable_string);
	fclose(fptr);
	return 0;
}



// ALERT: Test this function for error
char* read_nth_line(char* filename, int current_line_to_read)
{
	// reference : https://www.quora.com/How-do-I-read-nth-line-of-a-file-in-C-using-File-handling
	FILE* fptr = fopen(filename, "r");

	if(fptr == NULL)
	{
		printf("Error! File does not exist");
		exit(1);
	}

	char* ptr = (char* )malloc(1000 * sizeof(char));
	char line[1000];
	int i = 0;
	// first line of file is 1, not 0
	while (fgets(line, sizeof(line), fptr))
	{
		i++;
		if(i==current_line_to_read)
		{
			fclose(fptr);
			ptr = (char* )line;
			return ptr;
		}
	}

	return "ERROR. You requested a line greater than what exists in the file";
}



int getNumberofLinesInFile(char* filename)
{
	// reference: https://www.sanfoundry.com/c-program-number-lines-text-file/
	FILE *fileptr;
	int num_lines = 0;
	char chr;

	fileptr = fopen(filename, "r");

	//extract character from file and store in chr
	chr = getc(fileptr);
	while (chr != EOF)
	{
		//Count whenever new line is encountered
		if (chr == '\n')
		{
			num_lines = num_lines + 1;
		}
		//take next character from file.
		chr = getc(fileptr);
	}
	fclose(fileptr); //close file.
	return num_lines + 1;
}


///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////		THREAD FUNCS BELOW		///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


void* WriteToServer(void* data)
{
	// user_x = sender   username
	// user_y = reciever username
	char user_x[1000];
	strcpy(user_x, (char* )data);

	while (1)
	{
		// STEP 2: write to server if user presses ENTER

		// TYPES OF REQUESTS:
		// O|user_x|user_y|message		// user_x -> user_y		//O:One
		// A|user_x|message 			// user_x -> all 		//A:All
		// N|user_x 					// newuser 				//N:Newuser
		// U|undefined					// error 				//U:Undefined

		char inputtedMessage[1000]; // new memory?
		int didUserEnterAnything = scanf("%s", inputtedMessage);
		if(didUserEnterAnything == 1) // == 1 is also fine
		{
			char second_message[500];
			scanf("%[^\n]s", second_message);
			strcat(inputtedMessage, second_message);

			printf("Send to all or <enter username>?\n");
			char whoToSend[50];
			scanf("%s", whoToSend);

			char sendThisToServer[1200];

			if(strcmp(whoToSend, "all") == 0)
			{
				// send to all
				strcat(sendThisToServer, "A|");
				strcat(sendThisToServer, user_x);
				strcat(sendThisToServer, "|");
				strcat(sendThisToServer, inputtedMessage);
			}

			else
			{
				// send to one user
				strcat(sendThisToServer, "O|");
				strcat(sendThisToServer, user_x);
				strcat(sendThisToServer, "|");
				strcat(sendThisToServer, whoToSend);
				strcat(sendThisToServer, "|");
				strcat(sendThisToServer, inputtedMessage);
			}

			AppendToFile("server", sendThisToServer);
			printf("\n");
		}
	}
}



void* GetLatestLine(void* data)
{
	char username[1000];
	strcpy(username, (char* )data);
	int current_last_line = getNumberofLinesInFile(username);

	while (1)
	{
		// STEP 1: Check if anything was written to user's file, print it
		if(getNumberofLinesInFile(username) > current_last_line)
		{
			// getNumberofLinesInFile can only be 1 greater than current_last_line
			char* p_lineOfx = read_nth_line(username, getNumberofLinesInFile(username));
			char line_read[1000]; // newline character not included

			strcpy(line_read, p_lineOfx);
			printf("%s\n", line_read);

			current_last_line += 1;
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////		MAIN 		///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////



int main()
{
	printf("Welcome to the Awesome Messaging Service\n");
	char username[50];
	printf("Enter your username: ");
	scanf("%[^\n]", username);

	// ALERT: donot allow username to have spaces
	// ALERT: Username cannot contain '|'


	// CREATING THE TEXT FILE FOR THE USER BELOW:
	FILE* temp_fptr = fopen(username, "w"); 	// this creates the file, in same directory
	fclose(temp_fptr); 	// closes the file
	pthread_t thread1, thread2;

	int tid[] = {1, 2};

	// create threads
	pthread_create(&thread1, NULL, GetLatestLine, (void *)username);
	pthread_create(&thread2, NULL, WriteToServer, (void *)username);
	// wait for threads to finish their work
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	return 0;
}
