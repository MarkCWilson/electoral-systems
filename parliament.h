class Parliament{

private:
	vector<int> _v;
public:
	int get(int i);
	void set(int i, int n);
	int size();
	int numseats();
	Parliament();
	Parliament(vector<int> seats);
	Parliament(int m);
	Parliament(string file);
    void print();
    void print(vector<double> v);
    Parliament add(Parliament p2);
	double invLT();
	double LT();
	double LTSS();
	vector<double> SS();
	double l2dist(Parliament q);
	double l1dist(Parliament q);
};
