namespace Rules
{ 
	Parliament alloc_avg(vector<int> votes, int S, char alloc);
	Parliament alloc_rem(vector<int> votes, int S, char quota);
	Parliament alloc(vector<int> votes,  int S, char method);
	Parliament plurfam(const VoteMatrix &M);
	Parliament simple(const VoteMatrix &M, int d);
	Parliament simple(const VoteMatrix &M, int d, char method);
	Parliament FPP(const VoteMatrix &M);
	Parliament SM(const VoteMatrix &M, int d); 
	Parliament plurfam(const VoteMatrix &M, vector<int> L, vector<int> D);
	int winner (vector<int> c);
	vector<int> top (vector<int> c, int d);
	vector<int> scorevec(const Profile &p, vector<int> w);
	int scorewinner(const Profile &p, vector<int> w);
	vector<int> bordaweight(int m);
	vector<int> plurweight(int m);
};
