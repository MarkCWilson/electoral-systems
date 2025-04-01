class Profile { 
private:
	vector<vector<int> > _m;
public:
	int get(int r, int c) const;
	vector<int> getrow (int r) const;
	Profile();
	Profile(int N);	
	Profile(string file);
    Profile(int N, int m, char c);	
    Profile(int N, int m, double phi);
	void print() const;
	int numvoters() const;	
	int numcands() const;
	bool prefers (int r, int a, int b) const;
	void set(int r, int c, int n);
	void invert();
};