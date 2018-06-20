#include <sys/types.h>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <deque>

using namespace std;

// Function that pauses the output and requests user press [Enter]
void enterContinue(){
    cout << "Press [Enter] to continue.";			// Asks user to press [Enter]
    cin.get();										// Obtains user input
}

// Function that performs the increment/decrement
string numberChange(string numString, int changeBy){
    int num;											// Declare number variable
    if(numString[numString.length() - 1] == ','){		// Checks if string contains a comma
        numString.pop_back();							// Removes comma from string
        num = atoi(numString.c_str());					// Converts string to int
        return to_string(num + changeBy) + ',';			// Returns string with the changed number and replaces the comma
    }
    else if(numString[numString.length() - 1] == '.'){	// Checks if string contains a dot
        numString.pop_back();							// Removes dot
        num = atoi(numString.c_str());					// Converts string to int
        return to_string(num + changeBy) + '.';			// Returns string with changed number and replaces the dot
    }
    else{
        num = atoi(numString.c_str());					// Changes string to a number
        return to_string(num + changeBy);				// Returns the changed number
    }
}

// Function that determines if a string is a standalone number
bool isNum(string str){
    for(int i = 0; i < str.length(); i++){				// Iterates through string
        if(!isdigit(str[i])){							// If a character is not a digit
            return false;								// return false
        }
    }
    return true;
}

// Function that splits string by spaces and stores it in a deque
deque<string> splitString(string line){
    deque<string> storage;					// Declare deque to store words
    stringstream ss(line);					// Insert the line into a stream
    string token;							// Declare string variable to get words from line

    while(getline(ss, token, ' ')){			// Find words by using a blank space as a delimiter
        storage.push_back(token);			// Stores the word into a deque
    }

    return storage;							// return the deque
}

// Function that formats the text file
void formatText(fstream& readFile, fstream& writeFile){
    string line, word, toSend;		// Declare line to hold the line, word to hold inidivdual words, and toSend for the formatted line to output
    int charCount = 0;				// Initialize charCount to 0 to keep count of the current number of characters
    int tabCount = 0;				// Initialize tabCount to 0 to keep track of the current indentation
    deque<string> wordStor;			// Declare a deque to hold the words

    while(getline(readFile,line)) {							// Iterate through each line in the file
        if(wordStor.empty() && toSend.empty()){
            tabCount = line.find_first_not_of('\t');		// Calculate the current indentation if no words stored in the deque or toSend
        }
        else if(tabCount != line.find_first_not_of('\t')){	// If the indentation is different,
            writeFile << toSend << endl;					// write the current string to the file
            toSend = "";									// Empty the current string
            charCount = 0;									// Restart the character count
            tabCount = line.find_first_not_of('\t');		// Update the indentation
        }
        line.erase(0, tabCount);							// Remove the indentation (\t) in the line
        wordStor = splitString(line);						// Get the words in the line
        while(charCount <= 72){
            if(wordStor.empty()){							// Break when there are no more words to go through
                break;
            }
            if(toSend.empty()){								// Insert tabs into the current string if it is empty
                for(int i = 0; i < tabCount; i++){
                    toSend += '\t';
                    charCount += 1;
                }
            }

            word = wordStor.front();						// Get the first word in the deque
            wordStor.pop_front();							// Remove the word from the deque

            if((word.length() + charCount) == 72){			// If the current string will have 72 characters after adding the word,
                toSend += word;								// add the current word to the string,
                writeFile << toSend << endl;				// send it to the file,
                charCount = 0;								// reset the character count
                toSend = "";								// and empty the string
            }
            else if((word.length() + charCount) < 72) {		// If the current string will have less than 72 characters after adding the word,
                toSend += word + ' ';						// add the current word to the string,
                charCount += word.length() + 1;					// and update the character count
            }
            else{											// If the current string will have more than 72 characters after adding the word,
                writeFile << toSend << endl;				// send it to the file,
                toSend = "";								// empty the string,
                charCount = 0;
                for(int i = 0; i < tabCount; i++){			// reinsert the current indentation
                    toSend += '\t';
                    charCount += 1;
                }
                toSend += word + ' ';						// add the word to the next string to send
                charCount += word.length() + 1;				// and update the character count
            }
        }
        if(readFile.peek() == '\n'){						// Check next line for a blank line
            writeFile << toSend << "\n\n";					// send the current string if there is and an additional '\n' to insert a blank line,
            toSend = "";									// empty the string,
            charCount = 0;									// and restart the character count
        }
    }
    writeFile << toSend;									// Send remaining string to the file
}

// Function that copies the text from one file to another
void copyText(fstream& source, fstream& out){
    string line;								// Declare line to hold the line from the source
    while(getline(source, line)){				// Iterates through each line and sends it to the output file
        out << line << endl;
    }
}

// Function that performs the integer update (producer function)
void proFunction(fstream& file, int changeBy){
    string line, toWrite, word;							// Declare variables to hold the line from the file, the current word being processed, and the string to write to the file
    deque<string> wordStor;								// Declare the deque to hold the words

    fstream temp("temp.txt", fstream::in | fstream::out | fstream::trunc);				// Creates a temporary blank text file

    while(getline(file, line)){							// While the temp has lines for writing
        wordStor = splitString(line);					// Store the words of the line in the deque
        while(!wordStor.empty()){						// Iterate through the words
            word = wordStor.front();
            wordStor.pop_front();
            if(isNum(word) || (isdigit(word[word.length() - 2]) && (word[word.length() - 1] == ',' || word[word.length() - 1] == '.'))){	// Check if the word is an integer that may have a comma or dot attached
                word = numberChange(word, changeBy);	// Update the integer
            }
            toWrite += word + ' ';						// Update the current string
        }
        temp << toWrite << endl;						// Send the string to the file
        toWrite = "";									// Clear current string
    }

    temp.close();
    temp.clear();
}

int main(int argc, char* argv[]) {

    if(argc != 4){								// Check if there are enough arguments
        perror("Invalid number of arguments.");	// Print error message
        exit(1);								// Exit the program
    }
    string filename = argv[1];          		// Text file argument
    int x_value = atoi(argv[2]);         		// X value argument
    int changeValue = atoi(argv[3]);    		// Value for increment/decrement

    int fd[2];									// File descriptor to pipe
    char buffer[72];                            // Buffer for printing from pipe
    int nbytes;                                 // Bytes read from pipe
    int lineCount;                              // Count lines to pause at 20

    pid_t pid;									// Process id declaration

    fstream inFile(filename);				    // Opens the file for reading/writing
    string line;                                // Line to write to pipe

    if(pipe(fd) == -1){
        perror("Error creating pipe.");			// Print error if piping fails
        exit(1);								// Exit the program
    }

    fstream outFile("temp.txt", fstream::in | fstream::out | fstream::trunc);		// Opens a temporary text file

    for (int i = 0; i < x_value - 1; i++){						// Create X - 1 child processes
        pid = fork();
        if(pid < 0) {								            // Check if fork failed
            perror("Error creating child process");				// Print error
            exit(1);											// Exit
        } else if(pid == 0){									// If child process
            close(fd[0]);										// Close read end of pipe
            outFile.close();                                                                // Close and reopen files to refresh
            outFile.clear();
            outFile.open("temp.txt", fstream::in | fstream::out | fstream::trunc);
            inFile.close();
            inFile.clear();
            inFile.open("newfile.txt");
            proFunction(inFile, changeValue);	                                            // Perform producer functions
            inFile.close();
            inFile.clear();
            inFile.open("newfile.txt", fstream::in | fstream::out | fstream::trunc);
            copyText(outFile, inFile);                                                      // copy results to original text file
            if(i == x_value - 2){								                            // If it is the last child
                outFile.close();
                outFile.clear();
                outFile.open("temp.txt", fstream::in | fstream::out | fstream::trunc);		// Opens a temporary text file
                inFile.close();
                inFile.clear();
                inFile.open("newfile.txt");
                formatText(inFile, outFile);					                            // Formats the text file and place in temp file
                inFile.close();									                            // Close the file
                inFile.clear();
                inFile.open("newfile.txt", fstream::in | fstream::out | fstream::trunc);	// Reopen and clear the file
                outFile.close();
                outFile.clear();
                outFile.open("temp.txt");
                inFile.close();									                            // Close the file
                inFile.open("newfile.txt", fstream::in | fstream::out | fstream::trunc);	// Reopen and clear the file
                copyText(outFile, inFile);
                inFile.close();
                inFile.clear();
                inFile.open("newfile.txt");
                while(getline(inFile, line)){			                                    // Iterate through lines of the file
                    write(fd[1], line.c_str(), 72);	                                        // Write lines to the pipe
                }
                inFile.close();                                                             // Close file
                outFile.close();						    	                            // Close the temporary file
                remove("temp.txt");                                                         // Deletes temporary file

            }
            exit(0);											// Exit child process
        } else{
            wait(NULL);											// Wait for child processes complete
        }
    }


    int status;									// Current status of waiting for child
    for (int i = 0; i < x_value - 1; i++){		// Iterate through each child and wait for them to stop
        wait(&status);
    }


    close(fd[1]);                               // Close write file descriptor
    while(nbytes = read(fd[0], buffer, 72)){    // Iterate through pipe
        if(lineCount == 20){
            enterContinue();
            lineCount = 0;
        }
        buffer[nbytes] = '\0';
        cout << buffer << endl;                 // Write out to console
        lineCount++;
    }

    close(fd[0]);
}