#include "raymarch.h"
#include <process.h>
#include <Windows.h>
using namespace std;

raymarch* rm;
void executeThreads(void* arg);
void runMulti();

int main(int argc, char** argv) {
	char* arg;
	arg = argv[1];
	if (argc < 2) {
		cout << "Invalid File Name Please Try Again: ";
		cin >> arg;
	}
	rm = new raymarch(arg);
	runMulti();
	rm->endMultiRayMarch();
	// Close console
	char input = 'y';
	while (input == 'y' || input == 'Y') {
		cout << "Create Another? (Y for Yes): ";
		cin >> input;
		if (input == 'Y' || input == 'y') {
			cout << "Input File Name: ";
			cin >> arg;
			rm = new raymarch(arg);
			runMulti();
			rm->endMultiRayMarch();
		}
	}
}

void runMulti() {
	_beginthread(executeThreads,0,(void*)0);
	_beginthread(executeThreads,0,(void*)1);
	_beginthread(executeThreads,0,(void*)2);
	_beginthread(executeThreads,0,(void*)3);
}

void executeThreads(void *arg) {
	int start = (int) arg;
	rm->calculateValues(start);
}