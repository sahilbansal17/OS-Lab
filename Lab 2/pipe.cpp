#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <fstream>

using namespace std;

int main () {

	string file_name, line;
	cout << "Please enter the name of the file: ";
	cin >> file_name;

	int r1 = fork();
	
	vector <string> data;
	if (r1 == 0) {
		// child process

		// read data from the file
		ifstream fin;
		fin.open(file_name);
		while (fin) {
			getline(fin, line);
			data.push_back(line);
		}	
	}
	else {
		int t = wait(NULL);
	}

	int r2 = fork();

	if (r2 == 0) {
		// child process
	}
	else {
		
	}

	return 0;
}