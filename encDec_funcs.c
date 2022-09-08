#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "encDec_funcs.h"
#ifdef DEBUG
#define DEBUG 1
#else
#define DEBUG 0
#endif

//function to print out a help message should the help arguments be inputted
void help(){
	fprintf(stdout, "encDec performs an XOR-based encryption/decryption on the specified input file using the specified key file to produce the specified output file.\n");
	fprintf(stdout, "The format for the arguments is as follows: encDec -i|--input <input file> -o|--output <output file> -k|--key <key file>\n");
}

//Wrapper function for malloc that will throw an error in the case that the output of the call to malloc is null
void *Malloc(size_t bytes) {
	void *pointer = malloc(bytes);
	if(pointer == NULL){
		fprintf(stderr, "malloc error (%d): %s\n", errno, strerror(errno));
		exit(errno);
	}
	return pointer;
}

//wrapper for fopen that will throw an error in the case that a file is not able to be opened, otherwise, the space is freed
FILE *Fopen(const char *filename, const char *mode) {
	FILE *file = fopen(filename, mode);
	if(file == NULL) { 
		fprintf(stderr,"fopen error (%d): %s\n", errno, strerror(errno));
		exit(errno);
	}
	else{
		if(*filename) {
			free((char*)filename);
		}
	}
	return file;
}

//wrapper for fread that throws an error if the output of the fread is equal to 0, if there is an end-of-file indicator, or if there is an error indicator not equal to 0
size_t Fread(void *ptr, size_t size, size_t numbuff, FILE *stream) {
	size_t elements = fread(ptr, size, numbuff, stream);
	if(elements == 0 && !feof(stream) && ferror(stream)) {
		fprintf(stderr, "fread error(%d): %s\n", errno, strerror(errno));
		exit(errno);
	} 
	return elements;
}

//wrapper for fwrite that throws an error if the output of the fwrite is equal to 0
size_t Fwrite(const void *ptr, size_t size, size_t numbuff, FILE *stream) {
	size_t elements = fwrite(ptr, size, numbuff, stream);
	if(elements == 0) {
		if(ferror(stream)) {
			fprintf(stderr, "fwrite error(%d): %s\n", errno, strerror(errno));
			exit(errno);
		}
	}
	return elements;	
}

//wrapper for fwrite that throws an error if the fclose function is unsuccessful in closing file
void Fclose(FILE *stream) {
	if (stream && fclose(stream)) {
		fprintf(stderr, "fclose error(%d): %s\n", errno, strerror(errno));
		exit(errno);
	}
}

//function to read the key from keyFilePtr to be used in the encryption/decryption process of the input
void readKey(unsigned char *bufferPtr, size_t keyLength, FILE *keyFilePtr) {
	size_t numRead;//number of items read
	numRead = Fread(bufferPtr, sizeof(char), keyLength, keyFilePtr);
	if(numRead < keyLength) {
		fprintf(stderr, "readkey error (206): key is fewer bytes than expected (%zu vs. %zu)\n", numRead, keyLength);
		exit(206);
	}
}

/*Function effectuates the following steps in order to encrypt or decrypt the input
1)the input is read from the inputFilePtr using the Fread function
2)in a for loop, the unsigned char of the block ptr is raised to the power of the unsigned char of the key pointer
3)the newly encrypted or decrypted text is written using the Fwrite function
4)Should the number of bytes written be different from the number of bytes read, an error is thrown
*/
void encryptDecrypt(unsigned char *blockPtr, unsigned char *keyPtr, size_t blocksize, FILE *inputFilePtr, FILE *outputFilePtr) {
	size_t numWritten = 0;
	size_t bytesRead = 0;
	while(!feof(inputFilePtr)) {
		numWritten = 0;
		if((bytesRead = Fread(blockPtr, sizeof(char), blocksize, inputFilePtr))) {
			for(size_t i = 0; i < bytesRead; i++) {
				if(DEBUG) { 
					fprintf(stdout, "%c (%#04X) ^ %c (%#04X) = ", blockPtr[i], blockPtr[i], keyPtr[i], keyPtr[i]);
				}
				blockPtr[i] = blockPtr[i] ^ keyPtr[i];
				if(DEBUG) {
					fprintf(stdout, "%c (%#04X)\n", blockPtr[i], blockPtr[i]);
				}
			}
			numWritten = Fwrite(blockPtr, sizeof(char), bytesRead, outputFilePtr);
		}
		if(numWritten != bytesRead) {
			fprintf(stderr, "encrypt/decrypt error (207): unable to write all items read (%zu vs. %zu)\n", numWritten, bytesRead);
		}
	}
}

/*Function runs throw a for loop to check each of the arguments inputted when running the function
Should the arguments be valid, space is allocated for the ifile, ofile, and kfile char pointers and they are copied over
*/
void parseArgs(int argc, char *argv[], char **ifile, char **ofile, char **kfile) {
	//ints to store the index of where the arguments are in the file for the input, output, and key. This improves the functions organization and readability.
	int indexI = 0;
	int indexO = 0;
	int indexK = 0;
	for(int i = 1; i < argc; i++) {
		if (((strcmp(argv[i], "-i") == 0) || (strcmp(argv[i], "--input") == 0)) && indexI == 0) {
		//if argv[i] has a valid marker for input and there hasn't already been an input
			i++;
			//itterates in the array to ensure that there is a valid argument following the marker
			if (i < argc) {
			//if there is an argument following the marker, the index is given to indexI for the string to get space allocated and to be copied
				indexI = i;
			}
			else {
			//if there is no argument following the marker, an error is thrown
				fprintf(stderr, "command line error (203): no argument after '-i|--input'\n");
				exit(203);
			}
		}
		else if (((strcmp(argv[i], "-o") == 0) || (strcmp(argv[i], "--output")) == 0) && indexO == 0) {
		//if argv[i] has a valid marker for output and there hasn't already been an output
			i++;
			//itterates in the array to ensure that there is a vlaid argument following the marker
			if (i < argc) {
			//if there is an argument following the marker, the index is given to indexI for the string to get space allocated and to be copied
				indexO = i;
			}
			else {
			//if there is no argument following the marker, an error is thrown
			 	fprintf(stderr, "command line error (204): no argument after '-o|--output'\n");
			 	exit(204);
			}
		}
		else if (((strcmp(argv[i], "-k") == 0) || (strcmp(argv[i], "--key") == 0)) && indexK == 0) {
		//if argv[i] has a valid marker for the key and there hasn't already been a key
			i++;
			//itterates in the array to ensure that the is a valid argument following the marker
			if (i < argc) {
			//if there is an argument following the marker, the index is given to indexI for the string to get space allocated and to be copied
				indexK = i;
			}
			else {
			//if there is no argument following the marker, an error is thrown
				fprintf(stderr, "command line error (205):  no argument after '-k|--key'\n");
				exit(205); 
			}
		}
		else if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0)) {
		//if argv[i] has the marker for help
			help();
			//calls to the help function to print out the instructions on running the program
			exit(0);
			//exits the program
		}
		else {
		//if the argument inputted does not match any valid format an error is thrown
			fprintf(stderr, "command line error(201): incorrect/unexpected argument entered\n");
			exit(201);
		}
	}
	if (indexI * indexO * indexK == 0) {
	//checks to ensure that there has been input for the input, output, and key. If any were ommited, the value in the if statement would logically have to be 0.
		fprintf(stderr, "command line error (202): one or more required arguments not entered\n");
		exit(202);
	}
	//To further improve readability, the memory allocation and string copying are not done within the for loop. Instead, the indexes are used so this can be done sepearately.
	*ifile = (char *)Malloc(strlen(argv[indexI]) + 1);
	strcpy(*ifile, argv[indexI]);
	*ofile = (char *)Malloc(strlen(argv[indexO]) + 1);
	strcpy(*ofile, argv[indexO]);
	*kfile = (char *)Malloc(strlen(argv[indexK]) + 1);
	strcpy(*kfile, argv[indexK]);
}

