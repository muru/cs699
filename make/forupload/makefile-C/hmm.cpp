#include "hmm.h"

using namespace std;

typedef long double ld;

hmm::hmm()
{
	N = 0;
	A = nullptr;
	B = nullptr;
}

hmm::hmm(const unsigned n, string states[], string p, long double **A, long double *B)
{
	N = n;
	A = new ld[n][n];
	B = new ld[n];

	for (int i = 0; i < n; i++)
		M[states[i]] = i;
}

ld hmm::probability_of(unsigned n, string O[])
{
}
