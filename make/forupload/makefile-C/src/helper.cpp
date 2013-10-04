#include <hmm.h>
#include <iostream>
#include <map>

string* read_symbols(istream &in, const size_t T)
{
	string *symbols = new string[T];
	for (int t = 0; t < T; t++)
		in >> symbols[t];
	return symbols;
}

ostream& operator<<(ostream& stream,
    const map<string, unsigned>& map) {
  stream << '{';
  char comma[3] = {'\0', ' ', '\0'};
  for (const auto& pair : map) {
    stream << comma << '"' << pair.first << "\":\"" << pair.second << '"';
    comma[0] = ',';
  }
  stream << '}';
  return stream;
}

ostream & operator<< (ostream &out, hmm& model)
{
	out << "N = " << model.N << ", M = " << model.M << endl;
	out << "S = " << model.S << endl;
	out << "V = " << model.V << endl;
	return out;
}

unsigned max_probability(ld *dt, ld **A, unsigned j, size_t N)
{
	int i = 0, i_max = 0;
	ld maxp = dt[0]*A[0][j];
	for (i = 1; i < N; i++)
	{
		ld temp = dt[i]*A[i][j];
		if (maxp > temp)
		{
			maxp  = temp;
			i_max = i;
		}
	}
	return i_max;
}
