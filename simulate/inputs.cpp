#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
using namespace std;

#include "votematrix.h"
#include "profile.h"
#include "parliament.h"
#include "rules.h"
#include "extra.h"
#include "inputs.h"

VoteMatrix Inputs::genmatrix(int N, int m, char c)  //only do PE for now, use c later
{ 

	int t = 0;	
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	mt19937 gen(seed); // mersenne twister
	// geometric_distribution<int> dis (0.3);
	uniform_int_distribution<int> dis(0,m-1);
	
	VoteMatrix M(N,m);
	//initialize
	for (int i = 0; i < N; i++) 
	{
		for (int j = 0; j < m; j++) 
		{
			t = dis(gen);
			if (t >=m) 
			{
				t = 0;
			}
			M.set(i,t,1 + M.get(i,t));	
		}
	}
	//M.print();
		
	M.PE(N*N*N, 1,0.1); // have generated the society	
	//problem, how to return it!
	return M;
}

VoteMatrix Inputs::genmatrix(VoteMatrix old) {

	VoteMatrix M = old;
	M.percentmatrix(100); // seed the PE process with percentages from last election
	int N = M.numrows();	
	M.PE(old.numvoters(), 1, 0.3); // have generated the society	

return M;	
}
