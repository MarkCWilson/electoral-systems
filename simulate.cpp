#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>
#include <random>
using namespace std;

#include "votematrix.h"
#include "profile.h"
#include "parliament.h"
#include "inputs.h"
#include "rules.h"
#include "measures.h"
#include "extra.h"
#include "simulate.h"



vector<double> Simulate::analyse_alloc(VoteMatrix M, int S, string output) { // need to include all the other methods too

		Parliament p, p1,p2,p3,p4;
		vector<double> v;
		VoteMatrix M2 = M;
		int N = M.numrows();
		M2.aggregate(N);
		

		p1 = Rules::alloc_rem(M2.getrow(0), S,'D');
		p = p1;
		v.push_back(p.LT());
		v.push_back(p.LTSS());
		v.push_back(Measures::cosine(p, M2.getrow(0)));
		v.push_back(Measures::cosineSS(p.SS(), M2.getrow(0)));
		v.push_back(Measures::gallagher(p, M2.getrow(0)));
		v.push_back(Measures::gallagherSS(p.SS(), M2.getrow(0)));
		v.push_back(Measures::stlague(p, M2.getrow(0))); 
		v.push_back(Measures::stlagueSS(p.SS(), M2.getrow(0))); 
		v.push_back((double)Measures::govtsize(p.SS()));
		v.push_back(0);
		
	
		
		p2 =  Rules::alloc_rem(M2.getrow(0), S, 'H');
		p = p2;
		v.push_back(p.LT());
		v.push_back(p.LTSS());
		v.push_back(Measures::cosine(p, M2.getrow(0)));
		v.push_back(Measures::cosineSS(p.SS(), M2.getrow(0)));
		v.push_back(Measures::gallagher(p, M2.getrow(0)));
		v.push_back(Measures::gallagherSS(p.SS(), M2.getrow(0)));
		v.push_back(Measures::stlague(p, M2.getrow(0))); 
		v.push_back(Measures::stlagueSS(p.SS(), M2.getrow(0))); 
		v.push_back((double)Measures::govtsize(p.SS()));
		v.push_back(1);

		
		p3 =  Rules::alloc_avg(M2.getrow(0), S, 'W');
		p = p3;
		v.push_back(p.LT());
		v.push_back(p.LTSS());
		v.push_back(Measures::cosine(p, M2.getrow(0)));
		v.push_back(Measures::cosineSS(p.SS(), M2.getrow(0)));
		v.push_back(Measures::gallagher(p, M2.getrow(0)));
		v.push_back(Measures::gallagherSS(p.SS(), M2.getrow(0)));
		v.push_back(Measures::stlague(p, M2.getrow(0))); 
		v.push_back(Measures::stlagueSS(p.SS(), M2.getrow(0))); 
		v.push_back((double)Measures::govtsize(p.SS()));
		v.push_back(2);
		
		p4 =  Rules::alloc_avg(M2.getrow(0), S, 'J');
		p = p4;
		v.push_back(p.LT());
		v.push_back(p.LTSS());
		v.push_back(Measures::cosine(p, M2.getrow(0)));
		v.push_back(Measures::cosineSS(p.SS(), M2.getrow(0)));
		v.push_back(Measures::gallagher(p, M2.getrow(0)));
		v.push_back(Measures::gallagherSS(p.SS(), M2.getrow(0)));
		v.push_back(Measures::stlague(p, M2.getrow(0))); 
		v.push_back(Measures::stlagueSS(p.SS(), M2.getrow(0))); 
		v.push_back((double)Measures::govtsize(p.SS()));
		v.push_back(3);
		Extra::print(v);
		
		if(output == "verb") 
		{		
			cout << "Largest remainders " << endl;
			cout << "Droop quota " << endl;
			cout << "Parliament ";
			p1.print();
			cout << "LT = " << v[0] << endl;
			cout << "LTSS = " << v[1] << endl;
			cout << "cosine = " << v[2] << endl;
			cout << "cosineSS = " << v[3] << endl;
			cout << "gallagher = " << v[4] <<endl;
			cout << "gallagherSS = " << v[5] <<endl;
			cout << "St Lague = " << v[6] << endl;
			cout << "St Lague SS = " << v[7] << endl; 
			cout << "govt size = " << v[8] <<endl;
			
			cout << "Hare quota " << endl;
			cout << "Parliament ";
			p2.print();
			cout << "LT = " << v[10] << endl;
			cout << "LTSS = " << v[11] << endl;
			cout << "cosine = " << v[12] << endl;
			cout << "cosineSS = " << v[13] << endl;
			cout << "gallagher = " << v[14] <<endl;
			cout << "gallagherSS = " << v[15] <<endl;
			cout << "St Lague = " << v[16] << endl;
			cout << "St Lague SS = " << v[17] << endl; 
			cout << "govt size = " << v[18] <<endl;
		
			cout << "Highest averages " << endl;		
			cout << "Webster-Sainte-Lague " << endl;
			cout << "Parliament ";
			p3.print();
			cout << "LT = " << v[20] << endl;
			cout << "LTSS = " << v[21] << endl;
			cout << "cosine = " << v[22] << endl;
			cout << "cosineSS = " << v[23] << endl;
			cout << "gallagher = " << v[24] <<endl;
			cout << "gallagherSS = " << v[25] <<endl;
			cout << "St Lague = " << v[26] << endl;
			cout << "St Lague SS = " << v[27] << endl; 
			cout << "govt size = " << v[28] <<endl;
		
			cout << "Jefferson-D'Hondt " << endl;
			cout << "Parliament ";
			p4.print();
			cout << "LT = " << v[30] << endl;
			cout << "LTSS = " << v[31] << endl;
			cout << "cosine = " << v[32] << endl;
			cout << "cosineSS = " << v[33] << endl;
			cout << "gallagher = " << v[34] <<endl;
			cout << "gallagherSS = " << v[35] <<endl;
			cout << "St Lague = " << v[36] << endl;
			cout << "St Lague SS = " << v[37] << endl; 
			cout << "govt size = " << v[38] <<endl;
	
		return v;
		}
		else if (output == "summ") 
		{
			return v;		
		}
		return v;	
		
		
}	


vector<double> Simulate::analyse_simple(VoteMatrix M, int d, string output) {

	Parliament p;
	vector<double> v;
	VoteMatrix M2 = M;
	M2.aggregate(d);
		
	p = Rules::simple(M2,d);	
				
	v.push_back(p.LT());
	v.push_back(p.LTSS());
	v.push_back(Measures::cosine(p, M2.getrow(0)));
	v.push_back(Measures::cosineSS(p.SS(), M2.getrow(0)));
	v.push_back(Measures::gallagher(p, M2.getrow(0)));
	v.push_back(Measures::gallagherSS(p.SS(), M2.getrow(0)));
	v.push_back(Measures::stlague(p, M2.getrow(0))); 
	v.push_back(Measures::stlagueSS(p.SS(), M2.getrow(0))); 
	v.push_back((double)Measures::govtsize(p.SS()));
			
	v.push_back((double)d);
		
	if(output == "verb") 
	{
		cout << "Simple family " << endl;	
		cout << "Parliament ";
		p.print();
		cout << "d = " << v[9] << endl;
		cout << "LT = " << v[0] << endl;
		cout << "LTSS = " << v[1] << endl;
		cout << "cosine = " << v[2] << endl;
		cout << "cosineSS = " << v[3] << endl;
		cout << "gallagher = " << v[4] <<endl;
		cout << "gallagherSS = " << v[5] <<endl;
		cout << "St Lague = " << v[6] << endl;
		cout << "St Lague SS = " << v[7] << endl; 
		cout << "govt size = " << v[8] <<endl;
		
		return v;
	}
	else if (output == "summ") 
	{
		return v;		
	}	
	return v;
		
}	

vector<double> Simulate::analyse_threshold(VoteMatrix M, double t, string output) {

	Parliament p;
	vector<double> v;
	int N = M.numrows();
	VoteMatrix M2 = M;
	M2.threshold(t); // no checking for bad input
	M2.aggregate(N);
		
	p = Rules::simple(M2,N);	
				
	v.push_back(p.LT());
	v.push_back(p.LTSS());
	v.push_back(Measures::cosine(p, M2.getrow(0)));
	v.push_back(Measures::cosineSS(p.SS(), M2.getrow(0)));
	v.push_back(Measures::gallagher(p, M2.getrow(0)));
	v.push_back(Measures::gallagherSS(p.SS(), M2.getrow(0)));
	v.push_back(Measures::stlague(p, M2.getrow(0))); 
	v.push_back(Measures::stlagueSS(p.SS(), M2.getrow(0))); 
	v.push_back((double)Measures::govtsize(p.SS()));
			
	v.push_back(t);
		
	if(output == "verb") 
	{
		cout << "Simple family " << endl;	
		cout << "Parliament ";
		p.print();
		cout << "t = " << v[9] << endl;
		cout << "LT = " << v[0] << endl;
		cout << "LTSS = " << v[1] << endl;
		cout << "cosine = " << v[2] << endl;
		cout << "cosineSS = " << v[3] << endl;
		cout << "gallagher = " << v[4] <<endl;
		cout << "gallagherSS = " << v[5] <<endl;
		cout << "St Lague = " << v[6] << endl;
		cout << "St Lague SS = " << v[7] << endl; 
		cout << "govt size = " << v[8] <<endl;
		
		return v;
	}
	else if (output == "summ") 
	{
		return v;		
	}	
	return v;
		
}	

vector<double> Simulate::analyse_SM(VoteMatrix M, int d, string output) {

	Parliament p;
	vector<double> v;
	VoteMatrix M2 = M;
	//M.print();
	M2.aggregate(M.numrows());
		
	p = Rules::SM(M, d);	
				
	v.push_back(p.LT());
	v.push_back(p.LTSS());
	v.push_back(Measures::cosine(p, M2.getrow(0)));
	v.push_back(Measures::cosineSS(p.SS(), M2.getrow(0)));
	v.push_back(Measures::gallagher(p, M2.getrow(0)));
	v.push_back(Measures::gallagherSS(p.SS(), M2.getrow(0)));
	v.push_back(Measures::stlague(p, M2.getrow(0))); 
	v.push_back(Measures::stlagueSS(p.SS(), M2.getrow(0))); 
	v.push_back((double)Measures::govtsize(p.SS()));
			
	v.push_back(d);
		
	if(output == "verb") 
	{
		cout << "SM family " << endl;
		cout << "Party sizes " << endl;
		p.print();
		cout << "alpha = " << v[9] << endl;
		cout << "LT = " << v[0] << endl;
		cout << "LTSS = " << v[1] << endl;
		cout << "cosine = " << v[2] << endl;
		cout << "cosineSS = " << v[3] << endl;
		cout << "gallagher = " << v[4] <<endl;
		cout << "gallagherSS = " << v[5] <<endl;
		cout << "St Lague = " << v[6] << endl;
		cout << "St Lague SS = " << v[7] << endl; 
		cout << "govt size = " << v[8] <<endl;
		
		return v;
	}
	else if (output == "summ") 
	{
		return v;		
	}	
	return v;
		
}	

vector<vector<double> > Simulate::simulate_alloc(int N, int m, int n) { 
	
	vector<vector<double> > v;
	VoteMatrix M;
	vector<double> row = vector<double>(40,0);
	
	for(int i = 0; i < n; i++) 
	{
		M =Inputs::genmatrix(N,m,'c');
		for (int d = 1; d < N+1; d++)
		{
			row = analyse_alloc(M, d, "summ");
		}
	}
	return v;	
}

vector<vector<double> >  Simulate::simulate_simple(int N, int m, int n) { 
	
	vector<vector<double> > v;
	VoteMatrix M;
	vector<double> row = vector<double>(10,0);
		
	for(int i = 0; i < n; i++) 
	{		
		M = Inputs::genmatrix(N,m,'c');
		M.print();			
		for(int d = 1; d < N+1; d++)
		{	
			if(N%d == 0) 
			{		
				row = analyse_simple(M, d, "summ"); 
				v.push_back(row);
			}			
		}				
	}	
	return v;	
}

vector<vector<double> > Simulate::simulate_SM(int N, int m, int n) { 

	vector<vector<double> >v;
	VoteMatrix M;
	vector<double> row = vector<double>(10,0);
	
	for(int i = 0; i < n; i++) 
	{
		M = Inputs::genmatrix(N,m,'c');	
		for (int Np = 1; Np < N; Np++) 
		{
			    int a = N - Np;		// go from small to large in top tier					
				row = analyse_SM(M, a, "summ"); 
				v.push_back(row);
		}	
			
	}
	return v;
}

vector<vector<double> > Simulate::simulate_threshold(int N, int m, int n) { 

	vector<vector<double> >v;
	VoteMatrix M;
	vector<double> row = vector<double>(10,0);
	double t;
	
	for(int i = 0; i < n; i++) 
	{
		M = Inputs::genmatrix(N,m,'c');	
		for (int T = 0; T < 15; T++) // need to tidy and deal with non-divisors, etc
		{
			t = 0.01*T;			
			row = analyse_threshold(M, t, "summ"); 
			v.push_back(row);	
		}	
			
	}
	return v;
}

vector<Parliament> Simulate::analyse_swing(VoteMatrix M, VoteMatrix M2, string output) {
	VoteMatrix M3 = M;
	VoteMatrix M4 = M2;
	Parliament p;
	vector<Parliament> v;
	
	int Nold = M.numvoters();
	int Nnew = M2.numvoters();
	M4.aggregate(M2.numrows());
	M3.aggregate(M.numrows());
	//M3.print();
	//M4.print();
	
	
	vector<double> poll = vector<double>();
	for (int i = 0; i < M2.numcols(); i++) 
	{
		poll.push_back(M4.getrow(0)[i]/(double)Nnew);
	}
	
	vector<double>old = vector<double>();
	for (int i = 0; i < M.numcols(); i++) 
	{
		old.push_back(M3.getrow(0)[i]/(double)Nold);
	}
	
	Extra::print(old);
	Extra::print(poll);
	
	VoteMatrix Ms1 = M;
	VoteMatrix Ms2 = M;
	VoteMatrix Ms3 = M;
	
	p = Rules::FPP(M2);
	v.push_back(p);
	
	Ms1.swing(poll, "PLH"); 
	//Ms1.print();
	p = Rules::FPP(Ms1);
	v.push_back(p);
	//p.print();
	
	Ms2.swing(poll , "UNS"); 
	p = Rules::FPP(Ms2);
	v.push_back(p);
	//p.print();
	
	
	Ms3.swing(poll , "Q"); 
	p = Rules::FPP(Ms3);
	v.push_back(p);
	//p.print();
	
	return v;
}

// generate all the data for analysis

void Simulate::simulate(int N, int m, int n)
{
	vector<vector<double> > v;
	vector<vector<double> > w;
	vector<string> s;
	
	s.push_back("LT");
	s.push_back("LTSS");
	s.push_back("cosine");
	s.push_back("cosineSS");
	s.push_back("gallagher");
	s.push_back("gallagherSS");
	s.push_back("stlague");
	s.push_back("stlagueSS");
	s.push_back("govtsize");
	s.push_back("other_parameter");
	
	cout << "alloc simulation " << endl;
	cout << endl;
	Extra::print(s);
	v = Simulate::simulate_alloc(N,m,n);
	Extra::print(v);
	
	cout << "SM family simulation " << endl;
	cout << endl;
	Extra::print(s);
	w = Simulate::simulate_SM(N,m,n);
	Extra::print(w);
	
	cout << "simple family simulation " << endl;
	cout << endl;
	Extra::print(s);
	w = Simulate::simulate_simple(N,m,n);
	Extra::print(w);
}
