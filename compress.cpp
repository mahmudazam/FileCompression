#include <iostream>
#include <cstring>
#include <fstream>
#include "copy.h"
#include "Huffman.h"
#include "FrequencyList.h"
#include "LOCALE.h"

using namespace std;

int power(const int num, const int pow) {
	if(pow == 0) {
		return 1;
	} else {
		return num * power(num, pow - 1);
	}
}

void printBytes(char *message, int size) {
	if(message == NULL) return;
	if(size <= 0) return;
	for(int i = 0; i < size; i++) {
		cout << (int)message[i] << endl;
	}
}

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
	return decNum;
}

char *toBin(const char c) {
	int decNum = (int)c;
	if(decNum < 0) decNum = decNum + 255;
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
		if(count > 0) *size = count;
		else *size = 0;
		char *message = new char[count];
		for(int i = 0; i < *size; i++) {
			message[i] = temp[i];
		}
		delete [] temp;
		fin.close();
		return message;
	}
	else return NULL;
}

bool writeFile(char *file, const char *name, int size) {
	if(file == NULL) return false;
	if(size <= 0) return false;
	ofstream out;
	out.open(name, ios::out);
	if(out.is_open()) {
		out.write(file, (size));
		out.close();
		return true;
	}
	return false;
}

// Apparently working...
char *compressBinaryString(const char *message, int size, int *sizeOut) {
	if(message == NULL) return NULL;
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
			if(tempInt < 128) {
				comp[i] = (char)tempInt;
			} else {
				comp[i] = (char)(tempInt - 255);
			}
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
	if(tempInt < 128) {
		comp[i] = (char)tempInt;
	} else {
		comp[i] = (char)(tempInt - 255);
	}
	i++;
	*sizeOut = compSize;
	// printBytes(comp, strlen(compressedMessage));
	return comp;
}

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

// Apparently working...
char *compressMessage(const char *message, HuffmanCodec **h, int size, int *compSize) {
	if(message == NULL) return NULL;
	FrequencyList *fList = createFrequencyList(message);
	if(fList == NULL) return NULL;
	HuffmanTree *hTree = createHuffmanTree(fList);
	if(hTree == NULL) return NULL;
	HuffmanCodec *hcdc = createHuffmanCodec(hTree);
	if(hcdc == NULL) return NULL;
	char *cstringMessage = new char[size + 1];
	for(int i = 0; i < size; i++) cstringMessage[i] = message[i];
	cstringMessage[size] = '\0';
	char *encodedMessage = encode(hcdc, cstringMessage);
	char *compressedMessage = compressBinaryString(encodedMessage, strlen(encodedMessage), compSize);
	// char *compressedMessage = compressBinaryString(cstringMessage, strlen(cstringMessage), compSize);
	*h = hcdc;
	return compressedMessage;
}

char *decompressMessage(const char *message, int size, HuffmanCodec *h) {
	if(message == NULL) return NULL;
	if(h == NULL) return NULL;
	int decompSize = -1;
	char *binaryMessage = decompressToBinaryString(message, size);
	char *decompressedMessage = decode(h, binaryMessage);
	delete [] binaryMessage;
	return decompressedMessage;
}

bool compressFile(const char *path, HuffmanCodec **h) {
	if(path == NULL) return false;
	int size = -1;
	int compSize = -1;
	char *file = readFile(path, &size);
	if(file == NULL) return false;
	char *compressedFile = compressMessage(file, h, size, &compSize);
	if(compressedFile == NULL) return false;
	if(writeFile(compressedFile, "CompressedFile.txt", compSize)) {
		delete [] file;
		delete [] compressedFile;
		return true;
	}
	delete [] file;
	delete [] compressedFile;
	return false;
}

bool decompressFile(const char *path, HuffmanCodec *h) {
	if(path == NULL) return false;
	int size = -1;
	char *file = readFile(path, &size);
	if(file == NULL) return false;
	char *decompressedFile = decompressMessage(file, size, h);
	if(decompressedFile == NULL) return false;
	if(writeFile(decompressedFile, "DecompressedFile.txt", strlen(decompressedFile))) {
		delete [] file;
		delete [] decompressedFile;
		return true;
	}
	delete [] file;
	delete [] decompressedFile;
	return false;
}

int main() {
/*
	// 000000010000001000000011100
	int size = -1;
	int compSize = -1;
	char *file = readFile("short.txt", &size);
	cout << "File Size(bytes): "<< size << endl;
	// printBytes(file);
	
	char *compFile = compressBinaryString(file, size, &compSize);
	cout << "Compressed Size(bytes): " << compSize << endl;
	printBytes(compFile, compSize);
	writeFile(compFile, "compressedShort.txt",compSize);
	int compressedShortSize = -1;
	char *compressedShort = readFile("compressedShort.txt", &compressedShortSize);
	cout << "Compressed Size(bytes) of short.txt: " << compressedShortSize << endl;
	printBytes(compressedShort, compressedShortSize);
	char *decompFile = decompressToBinaryString(compFile, compSize);
	int decompSize = -1;
	if(decompFile != NULL) {
		cout << decompFile << endl;
		decompSize = strlen(decompFile);
	}
	cout << "Decompressed Size(bytes): " << decompSize << endl;
*/

	cout << "Enter path to file to compress: "  << endl;
	char *temp = new char[1000];
	cin >> temp;
	char *path = new char[strlen(temp) + 1];
	strcpy(path, temp);
	delete [] temp;
	HuffmanCodec *hcdc = NULL;
	compressFile(path, &hcdc);
	decompressFile("CompressedFile.txt", hcdc);
	return 0;

}
