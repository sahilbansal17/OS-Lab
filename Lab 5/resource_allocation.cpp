#include <iostream>
#include <vector>

using namespace std;

#define MAX 100

vector <int> max_resources, available, results;
class Process{
	// private:
	public:
		vector <int> max, current;
		vector <int> required() {
			int n = max.size();
			vector <int> res(n);
			for (int i = 0; i < n; i ++) {
				res[i] = max[i] - current[i];
			}
			return res;
		}
		// vector <int> available() {
		// 	int n = max.size();
		// 	vector <int> res;
		// 	for (int i = 0; i < n; i ++) {
		// 		res[i] = max_resources[i] - current[i];
		// 	}
		// 	return res;
		// }
		void setMax(vector <int> m) {
			for (int i = 0; i < m.size(); i ++) {
				max.push_back(m[i]);
			}
		}
		void setCurrent(vector <int> c) {
			for (int i = 0; i < c.size(); i ++) {
				current.push_back(c[i]);
			}
		}
		bool invalid() {
			int n = max.size();
			for (int i = 0 ; i < n; i ++) {
				if (current[i] > max[i]) {
					return 1;
				}
			}
			return 0;
		}
};

vector <Process> p(MAX);

bool solve (int cur, int total, vector <bool> done, vector <int> avl, vector <int> q) {
	bool all_done = 1;
	for (int i = 0; i < done.size(); i ++) {
		if (done[i] == 0) {
			all_done = 0;
		}
	}
	if (all_done == 1) {
		cout << "The initial allocation is SAFE.\nOne of the possible allocation is:\n";
		for (int i = 0; i < q.size(); i ++) {
			results.push_back(q[i]);
			cout << q[i] << " ";
		}
		cout << endl;
		return 1;
	}

	for (int i = 0; i < total; i ++) {
		if (!done[i]) {
			vector <int> req = p[i].required(), next;
			vector <bool> nd;
			nd = done;
			bool res = 1;
			for (int j = 0; j < req.size(); j ++) {
				if (req[j] > avl[j]) {
					res = 0;
				}
			}
			if (res == 1) {
				for (int j = 0; j < req.size(); j ++) {
					next.push_back(avl[i] + p[i].max[j]);
				}
				nd[i] = 1;
				q.push_back(i);
				int n = q.size();
				if (solve(cur + 1, total, nd, next, q) == 1) {
					// if (cur == 0) {
					// 	for (int i = 0; i < q.size(); i ++) {
					// 		cerr << q[i] << " ";
					// 	}
					// 	cerr << endl;
					// }
					return 1;
				}
				else {
					q.pop_back();
				}
			}
		}
	}

	return 0;
}

int main(int argc, char **argv) {

	if (argc != 2) {
		cout << "Usage of the program: ./resource_allocate <input file>\n";
		return 0;
	}
	freopen(argv[1], "r", stdin);

	int num_process, num_resource, val;
	cin >> num_process;
	cin >> num_resource;

	if (num_process <= 0 || num_resource <= 0) {
		cerr << "Number of processes or resources should be positive.\n";
		return 0;
	}

	for (int i = 0; i < num_resource; i ++) {
		cin >> val;
		max_resources.push_back(val);
	}

	vector <int> m(num_resource), c(num_resource);

	for (int i = 0; i < num_process; i ++) {
		for (int j = 0; j < num_resource; j ++) {
			cin >> val;
			if (val < 0) {
				cerr << "Process " << i + 1 << " should require positive no. of max resources\n";
				return 0;
			}
			m[j] = val;
		}
		p[i].setMax(m);
		for (int j = 0; j < num_resource; j ++) {
			cin >> val;
			if (val < 0) {
				cerr << "Process " << i + 1 << " should have positive current allocation.\n";
				return 0;
			}
			c[j] = val;
		}
		p[i].setCurrent(c);
		if (p[i].invalid()) {
			cerr << "Process " << i + 1 << " has current availablity more than required.\n";
			return 0;
		}
	}
	vector <int> used(num_resource, 0);

	for (int i = 0; i < num_process; i ++) {
		for (int j = 0; j < num_resource; j ++){
			used[j] += p[i].current[j];
			// cerr << p[i].current[j] << " ";
		}
		// cerr << endl;
	}

	for (int i = 0; i < num_resource; i ++) {
		if (max_resources[i] - used[i] < 0) {
			cerr << "Max resources for resource " << i + 1 << " is less than current used resources.\n";
			return 0;
		} 
		available.push_back(max_resources[i] - used[i]);
	}

	vector <bool> done(num_process, 0);
	vector <int> q;
	int res = solve(0, num_process, done, available, q);
	// cerr << res << endl;

	if (res == 0) {
		cout << "The initial allocation is UNSAFE.\n";
		return 0;
	}

	//*/
	cout << "\nNo. of allocated resources initially:\n";
	for (int i = 0; i < used.size(); i ++) {
		cout << used[i] << " ";
	}
	cout << endl;
	//*/

	//*/
	cout << "\nNo. of available resources initially:\n";
	for (int i = 0; i < available.size(); i ++) {
		cout << available[i] << " ";
	}
	cout << endl << endl;
	//*/

	printf("\t\t\t");
	for (int k = 0; k < num_resource; k ++) {
		printf("---Res[%d]---", k + 1);
	}
	cout << endl;
	printf("\t\t\t");	
	for (int k = 0; k < num_resource; k ++) {
		cout << "---Req---Avl";
	}
	cout << endl;

	for (int i = 0; i < results.size(); i ++) {
		int j = results[i];

		cout << "Process " << j ;
		vector <int> req = p[j].required();
		printf("\t\t");
		for (int k = 0; k < num_resource; k ++) {
			printf("%5d %6d", req[k], available[k]);
			available[k] = available[k] + p[j].max[k];
		}
		cout << endl;
	}

	/*/
	vector <int> queue;
	for (int k = 0; k < num_process; k ++) {
		for (int i = 0; i < k; i ++) {
			if (!done[i]) {
				vector <int> req = p[i].required();
				bool poss = 1;
				for (int j = 0; j < req.size(); j ++) {
					if (req[j] > available[j]) {
						poss = 0;
					}
				}
				if (poss == 1) {
					done[i] = 1;
					queue.push_back(i);
					for (int j = 0; j < req.size(); j ++) {
						available[j] += p[i].max[j];
						// cerr << available[j] << " ";
					}
					// cerr << endl;
					cerr << "Process " << i << "\n";
				}
			}
		}
	}
	//*/

	return 0;
}