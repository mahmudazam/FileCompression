#include "FrequencyList.h"

#include <iostream>
using namespace std;

int main() {
	// Seed rand():
	srand(time(NULL));

	// Set up a char array with random characters for testing:
	cout << "Size of message: " << endl;
	int size;
	cin >> size;
	char *message = new char[size];
	for(int i = 0; i < size; i++) {
		message[i] = rand() % 10 + 48;
		cout << "'" << message[i] << "'(" << (int)message[i] << "), ";
	}
	cout << endl;
	cout << message << endl;

	// Create a FrequencyList object:
	FrequencyList *fList = createFrequencyList(message, size);
	toString(fList);
	destroyFrequencyList(fList);
	delete [] message;
	return 0;
}
