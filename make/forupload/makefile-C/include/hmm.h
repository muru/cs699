#include <string>
#include <map>
#include <utility>
typedef long double ld;
using namespace std;

class hmm
{
	size_t N, M;
	long double **A, **B, *p;
	map<string, unsigned> S, V;
	map<unsigned, string> _S, _V;
public:
	~hmm()
	{
		delete[] A;
		delete[] B;
		delete[] p;
	}
	hmm(size_t n, size_t m, string states[], string events[], ld _p[], ld **_A, ld **_B);
	hmm(istream &in);
	ld probability_of(const unsigned T, string observation[]);
	string * state_sequence_for(const unsigned T, string observation[]);
	friend ostream& operator<< (ostream &, hmm &);
};

string* read_symbols(istream& in, const size_t T);
ostream& operator<<(ostream& stream, const map<string, unsigned>& map);

unsigned max_probability(ld *dt, ld **A, unsigned j, size_t N);
