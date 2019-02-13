#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <vector> 
#include <map>

using namespace std;


map <int, int> parent; // to store the parent pid of a process pid

vector <int> descendants (int pid) {
	vector <int> res;
	for (auto &it: parent) {
		if (it.second == pid) {
			res.push_back(it.first);
		}
	}
	return res;
}

void print_process (int pid, int ppid, vector <int> desc) {
	int n = desc.size();
	printf("pid: %d, ppid: %d, num_of_descendants: %d\n", pid, ppid, n);

	for (int i = 0 ; i < n; i ++) {
		printf("descendant %d pid: %d", i + 1, desc[i]);
	}
	printf("\n\n");
}

int main () {

	// first fork call
	int r1 = fork(), r2, r3;
	if (r1 < 0) {
		cout << "Fork1 failed" << endl;
		return 0;
	}

	if (r1 == 0) {
		// child
		parent[getpid()] = getppid();
		r2 = fork();
		if (r2 == 0) {
			parent[getpid()] = getppid();
		}
		else {
			int w2 = wait(NULL);
		}
	}
	else {
		// parent
		int w1 = wait(NULL);
		parent[getpid()] = getppid();
		r3 = fork();
		if (r3 == 0) {
			parent[getpid()] = getppid();
		}
		else {
			int w3 = wait(NULL);
		}
	}

	/*/
	vector <int> desc;
	desc = descendants(getpid());
	print_process(getpid(), getppid(), desc);
	//*/

	

	return 0;
}