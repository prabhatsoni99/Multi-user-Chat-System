#include <stdio.h>
#include <stdlib.h>  // For exit() function
#include <string.h>
#include <unistd.h> // for sleep

/*
Data is read from 	server 		file
when a new line is added. Thus, first line is useless and must
be initialised with some random line like "-----------"
2nd line must not begin!
*/

// add more documentation


#define LEN(arr) ((int) (sizeof (aarr) / sizeof (arr)[0]))
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
////////////////////////////////		MAIN 		///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


int main()
{
	printf("You are starting the server....\n");

	// we will only read from server file in this program
	FILE* fptr = fopen("server", "r");
	// first line is nothing is not a prob coz anyways it is first newline and then write

	// ALL INITIALISATION HAS BEEN DONE
	// AFTER THIS WE ENTER INFINITE WHILE LOOP

	int current_last_line_number = getNumberofLinesInFile("server");

	printf("%d lines right now in server file\n", current_last_line_number);

	char listUsernames[240][50]; // max of 240 users allowed
	int numberOfUsers = -1;

	while(1)
	{
		// IMPORTANT : everywhere it is <= numberOfUsers
		//							NOT <  numberOfUsers

		// INFINITE WHILE LOOP WHERE ALL THE SERVER DOES IS
		// If someone writes to server file, it processes that request

		if(getNumberofLinesInFile("server") > current_last_line_number)
		{
			// NOW WE HAVE TO PROCESS 1 LINE
			// getNumberofLinesInFile can only be 1 greater than current_last_line
			char* p_lineOfx = read_nth_line("server", getNumberofLinesInFile("server"));
			char line_read[1000]; // newline character not included

			strcpy(line_read, p_lineOfx);
			// line_read is now a character array containing the message
			printf("New request from user : %s\n", line_read);

			current_last_line_number += 1;


			// TYPES OF REQUESTS:
			// O|user_x|user_y|message		// user_x -> user_y		//O:One
			// A|user_x|message 			// user_x -> all 		//A:All
			// N|user_x 					// newuser 				//N:Newuser
			// U|undefined					// error 				//U:Undefined

			char typeOfRequest = 'U';
			typeOfRequest = (char)line_read[0];

			char request_array[4][1000];
			char *p;
			p = strtok(line_read, "|");
			strcpy(request_array[0], p);
			int ctr = 1;

			while(p!=NULL)
			{
				p = strtok (NULL, "|");
				if(p==NULL)
					break;
				strcpy(request_array[ctr], p);
				ctr++;
			}

			switch(typeOfRequest)
			{
				case 'O':
				{
					// O|user_x|user_y|message		// user_x -> user_y		//O:One
					// request_array[0] : O
					// request_array[1]	: user_x
					// request_array[2]	: user_y
					// request_array[3]	: message

					// Message format: user_x -> you only: message

					char message[1200]; // not same as message written above
					strcat(message, request_array[1]);
					strcat(message, " -> you only: ");
					strcat(message, request_array[3]);

					AppendToFile(request_array[2], message);

				}
				break;


				case 'A':
				{
					// A|user_x|message 			// user_x -> all 		//A:All
					// request_array[0] : A
					// request_array[1] : user_x
					// request_array[2] : message

					char message[1200]; // not same as message written above
					strcat(message, request_array[1]);
					strcat(message, " -> everyone: ");
					strcat(message, request_array[2]);

					for(int i=0;i<=numberOfUsers;i++)
					{
						printf("Message sent to %s\n", listUsernames[i]);
						AppendToFile(listUsernames[i], message);
					}
				}
				break;



				case 'N':
				{
					// N|user_x 					// newuser 				//N:Newuser
					// request_array[0] = N
					// request_array[1] = user_x

					printf("flag1\n");
					numberOfUsers += 1;
					strcpy(listUsernames[numberOfUsers], request_array[1]);

					char new_user[50];
					char message[1200];
					strcat(message, request_array[1]);

					strcat(message, " has joined the chat!");

					// send message to all users
					// including the user who joined as well
					for(int i=0;i<=numberOfUsers;i++)
					{
						printf("Message sent to %s\n", listUsernames[i]);
						AppendToFile(listUsernames[i], message);
					}

				}
				break;


				case 'U':
				{
					printf("Error. Cannot process undefined type of request [not A,O,N]\n");
				}
				break;
			}
		}

	}


	fclose(fptr); 	// closes the file
	return 0;

}
