#include <string>
#include <map>
using namespace std;
class hmm
{
	unsigned N;
	long double **A, *B;
	string p;
	map<string, unsigned> M;
	~hmm()
	{
		delete[] A;
		delete[] B;
		M.erase();
	}
public:
	hmm (const unsigned n, string N[], string p, long double **A, long double *B);
	hmm ();
	long double probability_of(unsigned n, string observation[]);
	string ** state_sequence_for(unsigned n, string observation[]);
};
