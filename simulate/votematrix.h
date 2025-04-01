using namespace std;

class VoteMatrix { 
private:
	vector<vector<int> > _m;
public:
	int get(int r, int c) const;
	void set(int r, int c, int n);
	vector<int> getrow (int r) const;
	VoteMatrix();
	VoteMatrix(int N, int m);
	VoteMatrix(int V, int N, int m); //IC generation
	VoteMatrix(string file);

	void print() const;
	int numvoters() const;
	int numvoters(int i) const;
	void aggregate(int d); 
	void percentmatrix(int k);
	void threshold(double t);
	void PE(int V, int K, double p);
	void swing(vector<double> newpoll, string t); 
	double frob(VoteMatrix M2);
	vector<double> poll();
	void add(VoteMatrix V);
	
	int numrows() const;
	int numcols() const;
};
