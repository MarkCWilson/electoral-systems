namespace Simulate
{	
	
	vector<double> analyse_alloc(VoteMatrix M, int S, string output);
	vector<double> analyse_threshold(VoteMatrix M, double t, string output);
	vector<double> analyse_simple(VoteMatrix M, int d, string output);
	vector<double> analyse_SM(VoteMatrix M, int d, string output);
	
	vector<vector<double> > simulate_alloc(int N, int m, int n);
	vector<vector<double> > simulate_threshold(int N, int m, int n);
	vector<vector<double> > simulate_simple(int N, int m, int n);
	vector<vector<double> > simulate_SM(int N, int m, int n);
	void simulate(int N, int m, int n);
	vector<Parliament> analyse_swing(VoteMatrix M, VoteMatrix M2, string output);
	
	
}
