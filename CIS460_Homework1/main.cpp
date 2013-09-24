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
	char* arg = new char[25];
	cout << "Input File Name: ";
	cin >> arg;
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
	thread t1(executeThreads,0);
	thread t2(executeThreads,1);
	thread t3(executeThreads,2);
	thread t4(executeThreads,3);
	thread t5(executeThreads,4);
	thread t6(executeThreads,5);
	thread t7(executeThreads,6);
	thread t8(executeThreads,7);
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	t7.join();
	t8.join();
	cout << "100%" << endl;
}
