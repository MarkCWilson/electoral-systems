#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
using namespace std;

#include "parliament.h"
#include "extra.h"

using namespace Extra;

//constructors

Parliament::Parliament() 
{
	
}

Parliament::Parliament(int m)
{
        _v.resize(m,0); 
}

Parliament::Parliament(vector<int> seats) 
{
	_v.resize(seats.size(),0);
	for (int i = 0; i < seats.size(); i++) 
	{
		_v[i] = seats[i];
	}
}

Parliament::Parliament(string file) 
{
	ifstream infile;
	infile.open(file);
	if(!infile.fail())
	{
 // need to fill in yet	
	}
}

//accessors

void Parliament::print() 
{
	cout << _v[0];
	for (int i = 1; i < _v.size(); i++) 
	{
		cout << " " << _v[i];	
	}
	cout << endl;
}

int Parliament::get(int i) 
{
	return _v[i];// need to do range checking for robustness
}


int Parliament::size() 
{
	return _v.size();
}

int Parliament::numseats() 
{
	int s=0;
	for (int i = 0; i < _v.size(); i++) 
	{
		s+=_v[i];
	}
	return s;
}

//mutators

void Parliament::set(int i, int n)
{
	_v[i] = n; // need to do range checking for robustness
}

Parliament Parliament::add(Parliament p2) 
{
	int m1 = this->size();
	Parliament p = *this;
	int m2 = p2.size();
	if(m1 == m2)
	{
		for (int i = 0; i < m1; i++) 
		{
			p.set(i, p.get(i) + p2.get(i));
		}		
	} 
	return p;
}

//functions

double Parliament::invLT()
{
	int m = this->size();
	double t = 0;
	for (int i = 0; i < m; i++) 
	{
		t+=this->get(i);
	}
	double s = 0;
	for (int i = 0; i < m; i++) 
		{
			s+=this->get(i)*this->get(i);	
		}			
	s = s/(t*t); // check for division by zero?
return m - 1/s; // check for division by zero? maybe return s?
}

double Parliament::LT()
{
	int m = this->size();
	double t = 0;
	for (int i = 0; i < m; i++) 
	{
		t+=this->get(i);
	}
	double s = 0;
	for (int i = 0; i < m; i++) 
		{
			s+=this->get(i)*this->get(i);	
		}			
	s = s/(t*t); // check for division by zero?
return 1/s; // check for division by zero? 
}

double Parliament::LTSS()
{
	vector<double> v = this->SS();
	double s = 0;
	for (int i = 0; i < v.size(); i++) 
		{
			s+=v[i]*v[i];
		}	
	return 1/s;
}


vector<double> Parliament::SS() // if vector is zero, it fails
{
	int m = this->size();
	int i = 0;
	vector<int> id = vector<int>(m,0);
	int t = 0;
	for (i = 0; i < m; i++) 
	{
		id[i] = i;
		t+= _v[i];
	}
	int q = (t+1)/2;
	vector<double> s(this->size(),0);
  	do {
    		t = 0;
    		i = 0;
    		//Extra::print(id);
    		while (t < q) 
    		{
    			t+= _v[id[i]]; // think it through!
    			i++;
    		}
    	s[id[i-1]]++;
 	   	} 
    while (next_permutation(id.begin(), id.end()) );
    for (i = 0; i < m; i++) 
    {
    	s[i] = s[i]/tgamma(m+1);    
    }
	return s; // not done yet
}

double Parliament::l2dist(Parliament q) // requires parliament dimension the same for both
{
	double d = 0;
	for (int i = 0; i < _v.size(); i++) 
		{
			d+=(_v[i] - q.get(i))*(_v[i] - q.get(i));
		}	
	return sqrt(d)/this->numseats();
}

double Parliament::l1dist(Parliament q) // requires parliament dimension the same for both
{
	double d = 0;
	for (int i = 0; i < _v.size(); i++) 
		{
			d+=abs(_v[i] - q.get(i));
		}	
	return d/this->numseats();
}
