// open any device file in binary and read only mode

#include <bits/stdc++.h>

using namespace std;

int main () {

	string file_name = "/dev/sdb";
	ifstream fin(file_name, ios::in | ios::binary);

	string block;

	char ch;
	int count = 0;
	while (count < 512) {
		fin >> ch;
		cout << ch;
		if (count % 80 == 0) {
			cout << endl;
		}
		count ++;
	}

	return 0;
}