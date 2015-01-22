#include <sys/wait.h>
#include <stdio.h>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <fcntl.h>
#define BUFFER_SIZE 100
#define INPUT_END 1
#define OUTPUT_END 0
using namespace std;

int main(void) {

        int userInput, input[2], output[2];
        char inputBuffer[BUFFER_SIZE] = "";
        char *outputBuffer = new char[100];

        pipe(input);
        pipe(output);

		fstream inputFileStream;
        inputFileStream.open("tasks.txt", ios::in);

		switch (fork()) {
		case -1:
			exit(1);

		case 0:

			while (1) {
				// Load file line
				inputFileStream.getline(outputBuffer, 20, '\n');
				if (inputFileStream.eof()) break;

				// Append newline
				outputBuffer[strlen(outputBuffer)] = '\n';
				cout << outputBuffer;
				cin >> userInput;

				// Close unneccesarry buffers
				close(input[OUTPUT_END]);
				close(output[INPUT_END]);

				// Send outputBuffer to child's input and read from its output
				write(input[INPUT_END], outputBuffer, strlen(outputBuffer));
				read(output[OUTPUT_END], inputBuffer, BUFFER_SIZE);

				if (atoi(inputBuffer) == userInput){
					cout << "ISPRAVNO" << endl;
				}
				else if (atoi(inputBuffer) != userInput) {
					cout << "NEISPRAVNO, tocan odgovor je: " << inputBuffer;
				}
				else {
					cout << "NEISPRAVAN IZRAZ" << endl;
				}
			}
			inputFileStream.close();
			break;

		default:
			close(input[INPUT_END]); // Because we are reading
			close(output[OUTPUT_END]); // Because we are writing
			dup2(input[OUTPUT_END], STDIN_FILENO);
			dup2(output[INPUT_END], STDOUT_FILENO);
			execlp("bc", "bc", NULL);

		}

        return 0;
}

