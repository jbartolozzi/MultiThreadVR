#include "raymarch.h"
#include <thread>
using namespace std;

raymarch* rm;
void executeThreads(int arg);
void runMulti();
int MAXWIDTH;


void executeThreads(int arg) {
	int start = arg;
	rm->calculateValues(start);
}

int main(int argc, char** argv) {
	char* arg;
	arg = argv[1];
	if (argc < 2) {
		cout << "Invalid File Name Please Try Again: ";
		cin >> arg;
	}
	rm = new raymarch(arg);
	MAXWIDTH = rm->IMAGEWIDTH;

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
			MAXWIDTH = rm->IMAGEWIDTH;
			runMulti();
			rm->endMultiRayMarch();
		}
	}
}

void runMulti() {
	cout << endl;
	cout << "Rendering: " << endl;
	for (int i = 0; i < MAXWIDTH;) {
		thread t1(executeThreads,i);
		thread t2(executeThreads,i+1);
		thread t3(executeThreads,i+2);
		thread t4(executeThreads,i+3);
		thread t5(executeThreads,i+4);
		thread t6(executeThreads,i+5);
		thread t7(executeThreads,i+6);
		thread t8(executeThreads,i+7);
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		t5.join();
		t6.join();
		t7.join();
		t8.join();
		if (i%200 == 0) {
			cout << 100*(((float)i)/((float)MAXWIDTH)) << "%, ";
		}
		i+=8;
	}
	cout << "100%" << endl;
}
