#include <vector>
#include <iostream>
#include <cmath>
using namespace std;
#include "parliament.h"
#include "measures.h"
#include "extra.h"

double Measures::gallagher(Parliament p, vector<int> votes) { 
	double g = 0;
	
	if (votes.size() == p.size()) 
	{
		int m = p.size();
		double v = 0;
		double s = 0;
		
		for (int i = 0; i < m; i++) 
		{
			v+=votes[i];
			s+=p.get(i);	
		}
		
		for (int i = 0; i < m; i++) 
		{
			g+=(votes[i]/v - p.get(i)/s)*(votes[i]/v - p.get(i)/s);	
		}			
	}
	else
	{
		;// what to do?
	}
	return sqrt(g)/2;	
}

double Measures::gallagherSS(vector<double>ss, vector<int> votes) {
	double g = 0;	
	
		int m = ss.size();
		double v = 0;
		double s = 0;
		
		for (int i = 0; i < m; i++) 
		{
			v+=votes[i];
			s+=ss[i];	
		}
		
		for (int i = 0; i < m; i++) 
		{
			g+=((ss[i]-votes[i]/v)*(ss[i]-votes[i]/v));	
		}			
		
	return sqrt(g)/2;	
}

double Measures::stlague(Parliament p, vector<int> votes) {
	double g = 0;
	
	if (votes.size() == p.size()) 
	{
		int m = p.size();
		double v = 0;
		double s = 0;
		
		for (int i = 0; i < m; i++) 
		{
			v+=votes[i];
			s+=p.get(i);	
		}
		
		for (int i = 0; i < m; i++) 
		{
			if (!(votes[i] == 0)) 
			{
				g+=v*(votes[i]/v - p.get(i)/s)*(votes[i]/v - p.get(i)/s)/votes[i];
			}	
			else
			{
				g+=p.get(i)*p.get(i)/(s*s);
			}
		}			
	}
	else
	{
		;// what to do?
	}
	return g;	
}

double Measures::stlagueSS(vector<double>ss, vector<int> votes) {
	double g = 0;
	
	if (votes.size() == ss.size()) 
	{
		int m = ss.size();
		double v = 0;
		double s = 0;
		
		for (int i = 0; i < m; i++) 
		{
			v+=votes[i];
			s+=ss[i];	
		}
		
		for (int i = 0; i < m; i++) 
		{
			if (!(votes[i] == 0)) 
			{
				g+=v*(votes[i]/v - ss[i]/s)*(votes[i]/v - ss[i]/s)/votes[i];
			}	
			else
			{
				g+=ss[i]*ss[i]/(s*s);
			}
		}			
	}
	else
	{
		;// what to do?
	}
	return g;	
}

double Measures::cosine(Parliament p, vector<int> votes) { // Koppel & Diskin, SCW 2009
	double g = 0;
	
	if (votes.size() == p.size()) 
	{
		int m = p.size();
		//convert p to a vector - maybe create method for this in Parliament class
		vector<double>pp;
		vector<double>vv(m);
		
		double v = 0;
		double s = 0;
		for (int i = 0; i < m; i++) // normalize to avoid overflow
		{
			v+=votes[i];
			s+=p.get(i);
		}
		
		for (int i = 0; i < m; i++) 
		{
			vv[i] = votes[i]/v;
			pp.push_back(p.get(i)/s);
		}
		
		v=0;
		s=0;
		for (int i = 0; i < m; i++) 
		{
			v+=vv[i]*vv[i];
			s+=pp[i]*pp[i];	
		}
		
		for (int i = 0; i < m; i++) 
		{
			g+=(vv[i]*pp[i]);	
		}
		g = 1 - g/sqrt(s)/sqrt(v);
	}
	else
	{
		;// what to do?
	}
	return g;	
}

double Measures::cosineSS(vector<double>ss, vector<int> votes) { // Koppel & Diskin, SCW 2009
	double g = 0;
	
	if (votes.size() == ss.size()) 
	{
		int m = ss.size();
		//convert p to a vector - maybe create method for this in Parliament class
		vector<double>pp(m);
		vector<double>vv(m);
		
		double v = 0;
		double s = 0;
		for (int i = 0; i < m; i++) // normalize to avoid overflow
		{
			v+=votes[i];
			s+=ss[i];
		}
		
		for (int i = 0; i < m; i++) 
		{
			vv[i] = votes[i]/v;
			pp[i] = ss[i]/s;
		}
		
		v=0;
		s=0;
		for (int i = 0; i < m; i++) 
		{
			v+=vv[i]*vv[i];
			s+=pp[i]*pp[i];	
		}
		
		for (int i = 0; i < m; i++) 
		{
			g+=(vv[i]*pp[i]);	
		}
		g = 1 - g/sqrt(s)/sqrt(v);
	}
	else
	{
		;// what to do?
	}
	return g;	
}

int Measures::govtsize(vector<double>ss) { // choose parties by decreasing SS index - need to improve
		double g = 0;	
		vector<double> s = ss;
		std::sort(s.begin(), s.end(), std::greater<double>()); 
		int m = s.size();
		
		for(int i = 0; i < m; i++) 
		{
			g = g + s[i];
			if (g > 0.5) 
			{
				return i+1;
			}
		}
		return m;		
}
