#include <iostream>
#include <hmm.h>
hmm::hmm(size_t n, size_t m, string states[], string events[], ld _p[], ld **_A, ld **_B)
	: N(n), M(m)
{
	int i, j;
	A = new ld*[N];
	B = new ld*[N];
	p = new ld[N];

	for (i = 0; i < N; i++)
	{
		S[states[i]] = i;
		_S[i] = states[i];
		p[i] = _p[i];
		A[i] = new ld[N];
		B[i] = new ld[M];
		for (j = 0; j < N; j++)
			A[i][j] = _A[i][j];
		for (j = 0; j < M; j++)
			B[i][j] = _B[i][j];
	}
	for (j = 0; j < M; j++)
	{
		V[events[j]] = j;
		_V[j] = events[j];
	}
}

hmm::hmm(istream& in)
{
	int i, j;
	in >> N >> M;
	A = new ld*[N];
	B = new ld*[N];
	p = new ld[N];
	string symbol;

	for (i = 0; i < N; i++)
	{
		in >> symbol;
		S[symbol] = i;
		_S[i] = symbol;
	}
	for (j = 0; j < M; j++)
	{
		in >> symbol;
		V[symbol] = j;
		_V[j] = symbol;
	}
	
	for (i = 0; i < N; i++)
		in >> p[i];

	for (i = 0; i < N; i++)
	{
		A[i] = new ld[N];
		for (j = 0; j < N; j++)
			in >> A[i][j];
	}

	for (i = 0; i < N; i++)
	{
		B[i] = new ld[M];
		for (j = 0; j < M; j++)
			in >> B[i][j];
	}
}

ld hmm::probability_of(const unsigned T, string observation[])
{
	int i, j, t;
	unsigned O[T];
	
	ld probability;								// P(O|\)
	ld a[T][N];									// alpha, the forward probability
	
	for (t = 0; t < T; t++)						
		O[t] = V[observation[t]];						// Indexes of the observation symbols.		

	// The Forward-Backward Procedure
	for (i = 0; i < N; i++)
	{
		cerr << (a[0][i] = p[i]*B[i][O[0]]) << endl;
	}

	for (t = 0; t < T - 1; t++)
	{
		for (j = 0; j < N; j++)
		{
			ld sum = 0;
			for (i = 0; i < N; i++)
			{
				sum += a[t][i] * A[i][j];
			}
			a[t+1][j] = sum * B[j][O[t+1]]; 
			cerr << a[t+1][j] << " ";
		}
		cerr << endl;
	}
	for (probability = i = 0; i < N; i++)
		probability += a[T-1][i];
	
	return probability;
}
	
string * hmm::state_sequence_for(const unsigned T, string observation[])
{
	int i, j, t;
	unsigned O[T], Q[T];
	ld P;
	ld d[T][N], psi[T][N];
	string *states = new string[T];

	for (t = 0; t < T; t++)						
		O[t] = V[observation[t]];						// Indexes of the observation symbols.		

	for (i = 0; i < N; i++)
	{
		d[0][i] = p[i]*O[i];
		psi[0][i] = 0;
	}

	for (t = 1; t < T; t++)
	{
		for (j = 0; j < N; j++)
		{
			unsigned maxp_i = max_probability(d[t-1], A, j, N);
			psi[t][j] = maxp_i;
			d[t][j]   = d[t-1][maxp_i] * A[maxp_i][j] * B[j][O[t]];
		}
	}
	P = d[T-1][0];
	Q[T-1] = 0;
	for (i = 1; i < N; i++)
		if (P > d[T-1][i])
		{
			P = d[T-1][i];
			Q[T-1] = i;
		}
	states[T-1] = _S[Q[T-1]];
	for (t = T-2; t >= 0; t--)
		states[t] = _S[Q[t] = psi[t+1][Q[t+1]]];

	return states;
}
