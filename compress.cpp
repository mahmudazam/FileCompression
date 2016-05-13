#include <iostream>
#include <cstring>
#include <fstream>
#include "copy.h"
#include "Huffman.h"
#include "FrequencyList.h"
#include "LOCALE.h"

using namespace std;

////////////////////////////////////////////////////
// Algorithm: power(num, pow)
// Raises num to the power pow.
// Pre: num::int, pow::int
// Post: nothing
// Return: int:: num raised to the power pow
int power(const int num, const int pow) {
	if(pow == 0) {
		return 1;
	} else {
		return num * power(num, pow - 1);
	}
}

////////////////////////////////////////////////////
// Algorithm: printBytes(message, size)
// Prints the decimal representation of every character in a character array, given the size of the array.
// Pre: message::char*, size::int:: the size of message
// Post: All characters in message are printed in decimal form.
// Return: nothing
void printBytes(char *message, int size) {
	cout << "File Size: " << size << endl;
	if(message == NULL) return;
	if(size < 0) return;
	cout << "File Bytes: " << endl;
	for(int i = 0; i < size; i++) {
		cout << (int)message[i]<< " ";
		if((i+1) % 25 == 0) cout << endl;
	}
	cout << endl;
}

////////////////////////////////////////////////////
// Algorithm toDec(num[8])
// Converts a string of '0'-s and '1'-s to a decimal number suitable for casting to a char.
// Pre: num::char[] of size 8
// Post: nothing
// Return: int:: a decimal number representing the binary number represented by num
int toDec(const char num[8]) {
	int decNum = 0;
	int index = 0;
	for(int i = 7; i > -1; i--) {
		if(num[i] == '1') {
			decNum += power(2, index);
			// cout << power(2, i)  << endl;
		} else if(num[i] == '0') {
			// cout << 0 << endl;
			decNum += 0;
		} else {
			return -999;
		}
		index++;
	}
	// cout << decNum << endl;
	if(decNum >= 128) decNum = decNum - 256;
	// cout << decNum << endl;
	return decNum;
}

////////////////////////////////////////////////////
// Algorithm: toBin(c)
// Converts a given character to a Cstring of '0'-s and '1'-s representing the given char in binary form.
// Pre: c::char
// Post: nothing
// Return: a Cstring of '0'-s and '1'-s of strlen() = 8, representing c in binary form
char *toBin(const char c) {
	int decNum = (int)c;
	if(decNum < 0) decNum = decNum + 256;
	char *binNum = new char[9];
	int i = 7;
	while(decNum != 0) {
		if(decNum % 2 == 0) binNum[i] = '0';
		if(decNum % 2 == 1) binNum[i] = '1';
		decNum = decNum / 2;
		i--;
	}
	while(i >=0 ) {
		binNum[i] = '0';
		// cout << binNum[i] << endl;
		i--;
	}
	binNum[8] = '\0';
	return binNum;
}

////////////////////////////////////////////////////
// Algorithm: readFile(path, size)
// Reads all characters in a file and records the number of bytes in the file.
// Pre: path::char* :: a Cstring representation of the path to a given file
//		size::int* : a reference to an int type variable where the size of the file
//					will be stored.
// Post: size dereferenced contains the size of the file read.
// Return: a char array containing all the characters in the file to which path leads
char *readFile(const char *path, int *size) {
	if(path == NULL) return NULL;
	ifstream fin;
	fin.open(path, ios::in);
	if(fin.is_open()) {
		int count = 0;
		char *temp = new char[1000000];
		fin.get(temp[count]);
		while(!fin.eof()) {
			count++;
			fin.get(temp[count]);
		}
		if(count > 0) {
			*size = count;
		}
		else *size = 0;
		char *message = new char[*size];
		for(int i = 0; i < *size; i++) {
			message[i] = temp[i];
		}
		delete [] temp;
		fin.close();
		return message;
	}
	else return NULL;
}

////////////////////////////////////////////////////
// Algorithm: writeFile(file, name, size)
// Writes a file containing all characters in a given char array.
// Pre: file::char* :: a char array containing characters to be written to a file
//		name::char* :: a Cstring representing the name of the file to be written
//		size::int :: the size of file
// Post: All characters in the char array file are written to a file with name name.
// Return: true if successful, false otherwise
bool writeFile(char *file, const char *name, int size) {
	cout << "Write File: File Size: " << size << endl;
	if(file == NULL) return false;
	if(size < 0) return false;
	ofstream out;
	out.open(name, ios::out);
	if(out.is_open()) {
		out.write(file, (size));
		// out.write((char) 10);
		out.close();
		return true;
	}
	return false;
}

////////////////////////////////////////////////////
// Algorithm: compressBinaryString(message, sizeOut)
// Compresses a Cstring containing '0'-s and '1'-s into a char array containing characters formed
//		by the bytes represented by the contiguous 8-char long substrings of the given Cstring
//		and records the size of the message after compression.
// Pre: message::char* :: a Cstring containing '0'-s and '1'-s
//		sizeOut::int* :: a reference to an int type variable to store the size of the compressed
//						message
// Post: A new char array containing the compressed message is created on the heap
//		and sizeOut dereferenced contains the size of the compressed message.
//		(This is necessary because '\0' termination is no longer valid for compressed message.)
// Return: char*:: a reference to a character array containing the compressed message
char *compressBinaryString(const char *message, int *sizeOut) {
	if(message == NULL) return NULL;
	int size = strlen(message);
	if(size < 8) return NULL;
	int tempInt = 256;
	char temp[8];
	char *comp;
	int compSize = -1;
	if(size % 8 == 0) {
		compSize = (size / 8) + 1;
	} else {
		compSize = (size / 8) + 2;
	}
	comp = new char[compSize];
	int i = 0;
	int index = 0;
	int j = 0;
	comp[i] = size % 8;
	i++;
	while(index < ((size / 8) * 8)) {
		temp[j] = message[index];
		index++;
		j++;
		if(j == 8) {
			j = 0;
			tempInt = toDec(temp);
			comp[i] = tempInt;
			i++;
		}
	}
	while(index < size) {
		temp[j] = message[index];
		j++;
		index++;
	}
	while(j < 8) {
		temp[j] = '0';
		j++;
	}
	tempInt = toDec(temp);
	comp[i] = tempInt;
	i++;
	*sizeOut = compSize;
	return comp;
}

////////////////////////////////////////////////////
// Algorithm: decompressToBinaryString(message, size)
// Decompresses a each character in a given char array to a Cstring representation of the char value in binary and 
//	concatenates these Cstrings to form a binary representation of the given char array.
// Pre: message::char* :: reference to a char array
//		size::int :: size of message
// Post: A new Cstring representing the given char array in binary is created on the heap
// Return: char*:: reference to a Cstring containing the decompressed message
char *decompressToBinaryString(const char *message, int size) {
	if(message == NULL) return NULL;
	if(size <= 0) return NULL;
	char *temp = NULL;
	int decompSize = ((size - 1) * 8) + 1;
	char *decomp = new char[decompSize];
	decomp[0] = '\0';
	int rem = (int)message[0];
	// cout << rem << endl;
	temp = toBin(message[1]);
	// cout << temp << endl;
	strcpy(decomp, temp);
	for(int i = 2; i < size ; i++) {
		temp = toBin(message[i]);
		// cout << temp << endl;
		strcat(decomp, temp);
		delete [] temp;
	}
	if(rem != 0) {
		decomp[((size - 1) * 8) - 8 + rem] = '\0';
	}
	char *decompressedMessage = copy(decomp);
	delete decomp;
	return decompressedMessage;
	return NULL;
}

////////////////////////////////////////////////////
// Algorithm: compressMessage(message, h, size, compSize)
// Compresses a given char array using a Huffman Codec.
// Pre: message::char* :: a reference to a char array
//		h::HuffmanCodec** :: a reference to a reference to a HuffmanCodec
//		size::int :: the size of message
//		compSize::int* :: a reference to an int type variable
// Post: A new char array containing the compressed message is created on the heap, 
//		h dereferenced contains a reference to the HuffmanCodec used to encode the message
//		compSize dereferenced contains the size of the compressed message
// Return: char*:: a reference to a char array containing the compressed message
char *compressMessage(const char *message, HuffmanCodec **h, int size, int *compSize) {
	
	// Convert message to a Cstring: (Not necessary after modification of encode function)
	char *cstringMessage = new char[size + 1];
	for(int i = 0; i < size; i++) cstringMessage[i] = message[i];
	cstringMessage[size] = '\0'; 
	
	// Create Huffman Codec:
	if(message == NULL) return NULL;
	FrequencyList *fList = createFrequencyList(message);
	if(fList == NULL) return NULL;
	HuffmanTree *hTree = createHuffmanTree(fList);
	if(hTree == NULL) return NULL;
	HuffmanCodec *hcdc = createHuffmanCodec(hTree);
	if(hcdc == NULL) return NULL;
		
	// Encode the Cstring message using the Huffman Codec:
	char *encodedMessage = encode(hcdc, message);
	// cout << encodedMessage << endl;
	char *compressedMessage = compressBinaryString(encodedMessage, compSize);
	*h = hcdc;
	return compressedMessage;
}

////////////////////////////////////////////////////
// Algorithm: decompressMessage(message, compSize, h, decompSize)
// Decompresses a given char array using a Huffman Codec.
// Pre: message::char* :: a reference to a char array
//		compSize::int :: the size of message
//		h::HuffmanCodec* :: a reference to a HuffmanCodec used to encode the given message
//		decompSize::int* :: a reference to an int type variable
// Post: A new char array containing the decompressed message is created on the heap, 
//		decompSize dereferenced contains the size of the decompressed message
// Return: char*:: a reference to a char array containing the compressed message
char *decompressMessage(const char *message, int compSize, HuffmanCodec *h, int *decompSize) {
	if(message == NULL) return NULL;
	if(h == NULL) return NULL;
	
	// Decompress the message: (No need to convert to a cstring) 
	char *binaryMessage = decompressToBinaryString(message, compSize); // returns a Cstring of '0'-s and '1'-s 
	// cout << "Test..." << endl;
	char *decompressedMessage = decode(h, binaryMessage); // decode the Cstring 
	*decompSize = strlen(decompressedMessage);
	// cout << "Test..." << endl;
	delete [] binaryMessage;
	return decompressedMessage;
}

////////////////////////////////////////////////////////////////////////////
// Algorithm: main(argc, argv)
// Tests all functions and runs a program to compress and decompress a file.
// Pre: argc::int :: number of command line arguments passed in
//		argv::char** :: a reference to an array of char arrays containing the
//						command line arguments
// Post: nothing
// Return: 0
int main(int argc, char **argv) {

////////////////////////////////////////////////////////////////////////////
/* Test code for readFile and writeFile:
	// 000000010000001000000011100
	int fileSize = -1;
	int binSize = -1;
	int compSize = -1;
	int decompSize = -1;
	char *binFile = readFile("message.txt", &fileSize);
	printBytes(binFile, fileSize);
	writeFile(binFile, "short2.txt", fileSize);
	char *binFile2 = readFile("short2.txt", &fileSize);
	printBytes(binFile2, fileSize); */

////////////////////////////////////////////////////////////////////////////
/* Test code for compressBinaryString and decompressToBinaryString: 
	int messageSize = -1;
	int compSize = -1;
	int decompSize = -1;
	char *message = "0011000100110010";
	messageSize = strlen(message);
	char *comp = compressBinaryString(message, &compSize);
	printBytes(comp, compSize);
	char *decomp = decompressToBinaryString(comp, compSize);
	cout << decomp << endl;
	decompSize = strlen(decomp);
	printBytes(decomp, decompSize); */

////////////////////////////////////////////////////////////////////////////
/*Test code for compressMessage and DecompressMessage: */
	int messageSize = -1;
	int compSize = -1;
	int decompSize = -1;
	
	char *message = readFile(argv[1], &messageSize);
	// printBytes(message, messageSize);
	cout << "Message Size(Bytes): " << messageSize << endl;
	HuffmanCodec *hcdc = NULL;
	char *comp = compressMessage(message, &hcdc, messageSize, &compSize);
	char *compPath = new char[strlen(argv[1]) + 6];
	strcpy(compPath, "comp-");
	strcat(compPath, argv[1]);
	writeFile(comp, compPath, compSize);
	
	// printBytes(comp, compSize);
	cout << "Message Size(Bytes) after compression: " << compSize << endl;
	char *decomp = decompressMessage(comp, compSize, hcdc, &decompSize);
	decompSize = strlen(decomp);
	// printBytes(decomp, decompSize);
	
	char *decompPath = new char[strlen(argv[1]) + 8];
	strcpy(decompPath, "decomp-");
	strcat(decompPath, argv[1]);
	writeFile(decomp, decompPath, decompSize);
	cout << "Message Size(Bytes) after decompression: " << decompSize << endl;
	
/*
	if(strcmp(message, decomp) == 0) {
		cout << endl << "The decompressed message and the original message are the same. " << endl << endl;
	} else {
		cout << endl << "The decompressed message and the original message are NOT the same. " << endl << endl;
	}
*/

////////////////////////////////////////////////////////////////////////////
/* Test code for final process: (Unused)
	cout << "Enter path to file to compress: "  << endl;
	char *temp = new char[1000];
	cin >> temp;
	char *path = new char[strlen(temp) + 1];
	path[0] = '\0';
	strcpy(path, temp);
	delete [] temp;
	HuffmanCodec *hcdc = NULL;
	compressFile(path, &hcdc);
	decompressFile(path, hcdc);
	int fileSize = -1;
	char *file = readFile(path, &fileSize);
	int decompSize = -1;
	char *decompressedFile = readFile(path, &decompSize);
	if(strcmp(file, decompressedFile) == 0) {
		cout << endl << "The decompressed file and the original file are the same. " << endl << endl;
	} else {
		cout << endl << "The decompressed file and the original file are NOT the same. " << endl << endl;
	} */

////////////////////////////////////////////////////////////////////////////
// Return normal exit code:
	return 0;

}

/*
// Unused functions:
bool compressFile(const char *path, HuffmanCodec **h) {

	// Read the file to which path leads: 
	if(path == NULL) return false;
	int size = -1;
	int compSize = -1;
	char *file = readFile(path, &size); // file contains all characters in the file to which path leads.
	if(file == NULL) return false;
	
	// Compress the file: 
	char *compressedFile = compressMessage(file, h, size, &compSize);
	if(compressedFile == NULL) return false;
	
	// Write the compressed version of the file: 
	char *newPath = new char[strlen(path) + 12];
	newPath[0] = '\0';
	strcpy(newPath, "Compressed-");
	strcat(newPath, path);
	if(writeFile(compressedFile, newPath, compSize)) {
		delete [] file;
		delete [] compressedFile;
		delete [] newPath;
		return true;
	}
	
	// Clean up in case the function fails: 
	delete [] file;
	delete [] compressedFile;
	delete [] newPath;
	return false;

}

bool decompressFile(const char *path, HuffmanCodec *h) {

	// Read the file to which path leads:
	if(path == NULL) return false;
	int compSize = -1;
	char *file = readFile(path, &compSize);
	if(file == NULL) return false;
	
	// Decompress the file: 
	int decompSize = -1;
	char *decompressedFile = decompressMessage(file, compSize, h, &decompSize);
	if(decompressedFile == NULL) return false;
	
	// Write the decompressed version of the file: 
	char *newPath = new char[strlen(path) + 14];
	newPath[0] = '\0';
	strcpy(newPath, "Decompressed-");
	strcat(newPath, path);
	if(writeFile(decompressedFile, newPath, decompSize)) {
		delete [] file;
		delete [] decompressedFile;
		delete [] newPath;
		return true;
	}
	
	// Clean up in case the function fails: 
	delete [] file;
	delete [] decompressedFile;
	delete [] newPath;
	return false;

}

*/

