#include <iostream>
#include <fstream>
#include <hmm.h>

using namespace std;

int main(int argc, char* argv[])
{
	if (argc < 2)
		return 1;
	size_t N, M, T;
	ifstream fin(argv[1]);
	hmm model(fin);
	cerr << model << endl;
	fin >> T;
	string *observation = read_symbols(fin, T);
	cout << model.probability_of(T, observation) << endl;
	string *states = model.state_sequence_for(T, observation);
	for (int t = 0; t < T; t++)
		cout << t << "\t" << states[t] << endl;
	
	return 0;
}
