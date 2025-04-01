#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
#include "votematrix.h"
#include "profile.h"
#include "parliament.h"
#include "rules.h"
#include "extra.h"

int Rules::winner(vector<int> c) //c is the score vector
{
	int max = 0; int maxi = c.size() - 1; // lex tiebreaking
	for (int i = 0; i<c.size(); i++) 
	{
		if ((c[i] > max) ||  (c[i] == max && i < maxi)) 
		{
			max = c[i];
			maxi = i;
		//	cout <<"Current max " << max << endl;
		//	cout <<"Current maxi " << maxi << endl;
		}
	}	
	
//	cout << "Winner  " << maxi << endl;
return maxi;
}

vector<int> Rules::top (vector<int> c, int d) 
{
	vector<int> t;
	sort(c.begin(), c.end());	
	for (int i = c.size() - 1; i > -1; i--) 
	{
 		t.push_back(c[i]);	
	}
return t;
}

vector<int> Rules::scorevec(const Profile &p, vector<int> w) 
{
	int m = p.numcands();// 
	w.resize(m);// this may give odd results
	vector<int> c = vector<int>(m,0);
	for (int i = 0; i < p.numvoters(); i++) 
	{
		for  (int j = 0; j < p.getrow(i).size(); j++) 
		{
			c[p.get(i,j)]+= w[j];	
		}	
	}
return c;	
}

int Rules::scorewinner(const Profile &p, vector<int> w)
{
		return Rules::winner(Rules::scorevec(p, w));
}
	
vector<int> Rules::bordaweight(int m)
{
	vector<int> w;
	for (int i = 0; i < m; i++) 
	{
		w.push_back(m-1-i);		
	}
return w;
}	

vector<int> Rules::plurweight(int m)
{
	vector<int> w=vector<int>(m,0);
	w[0] = 1;
return w;
}	



Parliament Rules::FPP(const VoteMatrix &M) // for double checking - should be same as plurfam with right arguments
{
	vector<int> votes;
	int w;
	Parliament p(M.numcols());
	
	for(int i = 0; i < M.numrows(); i++) 
	{
		votes = M.getrow(i);
		//cout << "District " << i << endl;
		//cout << "Entering with " << endl;
		//Extra::print(votes);
		w = winner(votes);
		p.set(w, p.get(w) + 1);
		
	}
return p;
}

Parliament Rules::alloc_avg(vector<int> votes,  int S, char alloc)
{
	int m = votes.size();
	vector<int> p = vector<int>(m,0);

		int u = 0;
		double q,k;
		vector<double> divisors = vector<double>(m,0);
		for (int j = 0; j < m; j++) 
			{
			switch(alloc)
				{
				case 'J': divisors[j] = 1; break; //Jefferson-d'Hondt
				case 'W': divisors[j] = 0.5; break; //Webster-St Lague
				case 'A': //Adams
				case 'D': //Dean
				case 'H': //Huntington-Hill
				case 'L': divisors[j] = 0; break; // Wada logarithmic
				}
			}

			for (int i = 0; i < S;)
				{
				k = 0;
                        
               	
                        for (int j = 0; j < m; j++)// find party with highest average so far
                        {
                                if (divisors[j] == 0.0 && votes[j] > 0) 
                                {
                                	u = j;
                                	break;// jump to increment seat if divisor is zero
                                }   
                                else
                                {                
                                	q = divisors[j]==0?0:votes[j]/divisors[j];
                                	if(q>k)
                                	{
                                    	k = q;
                                    	u = j;
                                	}
                                }
  
                        }
                
                p[u] = p[u] + 1;// add seat to party u
                //update next unused divisor for u
                switch(alloc)
               	 	{
                	case 'J': divisors[u] = p[u] + 1; break;
                	case 'W': divisors[u] = p[u] + 0.5; break; 
                	case 'A': divisors[u] = p[u]; break;
                	case 'D': divisors[u] =( (double)p[u]* ((double)p[u]+1))/((double)p[u]+0.5); break;
                	case 'H': divisors[u] = sqrt(p[u]*(p[u]+1)); break;
                	case 'L': divisors[u] =  1/(log(p[u]+1) - log(p[u]));
                	}
                
                ++i; // next seat
                                
                }

	return Parliament(p);
}

Parliament Rules::alloc_rem(vector<int> votes, int S, char quota) 
{
	int i = 0;
	int m = votes.size();
	vector<int> p = vector<int>(m,0);
	double q[m];//quota
	vector<double> r(m,0);//remainders
	double V = 0; // find total votes
	double d; // divisor to make quota
	
	for (i = 0; i < m; i++) 
	{
		V+=votes[i];
	}
	
	int s = 0;
	switch(quota) 
		{
		case 'H': d = V/S; break; //Hare
		case 'D': d = floor(1+V/(1+S)); break;	//Droop	
		}
		
	
	for (i = 0; i< m; i++) 
	{
		q[i] = votes[i]/d;
		p[i] = p[i] + floor(q[i]);
		s = s + floor(q[i]);
		r[i] = q[i] - floor(q[i]);	
	}	
		
	sort(r.begin(), r.end(), greater<double>()); 
				 
	i = 0;

	while(s < S) 
	{
		p[i]++;
		s++;
		i++;	
	}	
	return Parliament(p);	
	        
}

Parliament Rules::alloc(vector<int> votes,  int S, char method)
{
switch(method) 
		{
		case 'H': return alloc_rem(votes, S, 'H'); break; //Hare
		case 'D': return alloc_rem(votes, S, 'D'); break;	//Droop	
		case 'W': return alloc_avg(votes, S, 'W'); break; //Hare
		case 'J': return alloc_avg(votes, S, 'J'); break;	//Droop		
		}
return Parliament();
}

Parliament Rules::simple(const VoteMatrix &M, int d, char method) // elects d from each district
{
	vector<int> votes;
	Parliament p(M.numcols());
	Parliament q(M.numcols());
	
	for(int i = 0; i < M.numrows(); i++) 
	{
		votes = M.getrow(i);
		q = alloc(votes, d, method);//no checking for bad input
		p = p.add(q);
	}
	return p;
}

Parliament Rules::simple(const VoteMatrix &M, int d) // elects d from each district
{
	return Rules::simple(M,d,'W');
}

Parliament Rules::plurfam(const VoteMatrix &M, vector<int> L, vector<int> D) //multilevel system
//D contains magnitudes, L the number to aggregate at each level
{
	int l = L.size();
	int N = M.numrows();
	int m = M.numcols();
	Parliament p(m);
	Parliament q(m);
	VoteMatrix M2;
	
	for (int k = 0; k < l; k++) 
	{
		M2 = M;
		//cout << " L[k] " << L[k] << " D[k] " << D[k] << endl;
		M2.aggregate(L[k]);	
		//cout << M2.numrows() << " " << M2.numvoters() << endl;
		q = simple(M2, D[k]);
		p = p.add(q);
	}
	return p;
}

Parliament Rules::SM(const VoteMatrix &M, int d) // MMM (Supplementary Member)
{
	int N = M.numrows(); //number of districts
	 // d is magnitude of the top tier
	vector<int> L, D;
	L.push_back(1);
	L.push_back(N);
	D.push_back(1);
	D.push_back(d);
	
	return plurfam(M, L, D);
}
