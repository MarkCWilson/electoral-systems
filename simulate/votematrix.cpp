#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <random>
using namespace std;
#include "votematrix.h"
#include "parliament.h"
#include "extra.h"
#include "random.h"
#include "profile.h"
#include "rules.h"


//constructors

VoteMatrix::VoteMatrix() 
{
	
}

VoteMatrix::VoteMatrix(int N, int m)
{
        _m.resize(N, vector<int>(m,0));  // create and assign zero matrix
}

VoteMatrix::VoteMatrix(int V, int N, int m) //IC generation - fix so uses random.h
{
	 _m.resize(N, vector<int>(m,0));


	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine gen(seed); // try mersenne twister
   	uniform_int_distribution<int> district(0,N-1);
   	uniform_int_distribution<int> party(0,m-1);
	int k = 0; int l = 0; int v = 0;

	while(v < V)
	{
				k = district(gen);
				l =  party(gen);
				_m[k][l]++;
		v++;
	}	
}

VoteMatrix::VoteMatrix(string file) //reads matrix in CSV format - check format!
{
	ifstream infile;
	string line;
	char sep = ',';
	
	infile.open(file);
	
	if(infile.is_open())
	{
		while(!infile.eof()) 
		{							
		    std::getline(infile,line); 
		    if (!line.empty())
		    {		    
		    	_m.push_back(vector<int>(0));	
		    	istringstream iss(line);   			
	    		string token;	    		
	    
	    		while(std::getline(iss,token,sep))
	    		{	       
	        		_m.back().push_back(atoi(token.c_str())); // push numbers into vector
	    		}	   
			}			
		}		
 	infile.close();
	}
	else 
	{
		cout << "Unable to open file";	
	}	
}  


//accessors

int VoteMatrix::get(int r, int c) const
{
	return _m[r][c]; // need to do range checking for robustness
}

vector<int> VoteMatrix::getrow (int r) const
{
	vector<int> row;
	for (int j = 0; j < _m[0].size(); j++) 
	{
		row.push_back(_m[r][j]);		
	}
	return row;// need to do range checking for robustness
}

int VoteMatrix::numcols() const
{
	return _m[0].size(); 
}

int VoteMatrix::numrows() const
{
	return _m.size(); 
}

//number of votes in whole election
int VoteMatrix::numvoters() const
{
	int N = _m.size();
	int m = _m[0].size(); // what to do if empty?
	int c = 0;
	for(int j = 0; j < m; j++) 
	{
		for(int i = 0; i < N; i++) 
		{
			c+=_m[i][j];
		} 	
	}

	return c;
}

// number of votes in a given district
int VoteMatrix::numvoters(int i) const // range checking for i?
{
	int N = _m.size();
	int m = _m[0].size(); // what to do if empty?
	int c = 0;
	for(int j = 0; j < m; j++) 
	{		
			c+=_m[i][j];	
	}
	
	return c;
}

//mutators

void VoteMatrix::set(int r, int c, int n)
{
	_m[r][c] = n; // need to do range checking for robustness
}

void VoteMatrix::add(VoteMatrix V) //no range check yet
{	
	for(int i = 0; i < _m.size(); i++)
	    for (int j = 0; j<_m[0].size(); j++) 
	    {
		    _m[i][j]+=V.get(i,j);
        }
}

void VoteMatrix::percentmatrix(int k) // always returns at least 1% for a party
{
	int N = _m.size();
	int m = _m[0].size(); // what to do if m=0?
	int n;
	Parliament p;
	
	for (int i = 0; i < N; i++) 
	{
		n = VoteMatrix::numvoters(i);
		
		p = Rules::alloc_avg(getrow(i), k, 'W');
		
		for (int j = 0; j < m; j++) 
		{
			//_m[i][j] = _m[i][j]*k/n; 	// primitive!
			_m[i][j] = p.get(j);	
			/*
			if (_m[i][j] == 0)
			{
				_m[i][j] = 1;
			}
			*/
		}
	}
}

void VoteMatrix::aggregate(int d) 
{
	int N = _m.size();
	int m = _m[0].size(); // what to do if m=0?
	int k;
	
	// what to do if N/d = 0?
	if (d > N) {
		d = N;
	}
	
	if(N%d == 0) 
	{				
		for(int i = N/d; i < N; i++) 
		{	
			k = i%(N/d);
			for(int j = 0; j < m; j++) 
			{	
				_m[k][j] = _m[k][j] + _m[i][j];		
			} 	
		}
	}
	else
	{
	// need to do something, but what?
	}
	_m.resize(N/d);
}

void VoteMatrix::threshold(double t) //overall party vote threshold
{
	int N = _m.size();
	int m = _m[0].size(); // what to do if m=0?
	VoteMatrix M = *this;
	VoteMatrix M2 = M;
	M2.aggregate(N);
	int n = M.numvoters();
		for (int j = 0; j < m; j++) 
		{
			if (M2.get(0,j) < t*n) 
			{
				for (int i = 0; i < N; i++) 
				{
				_m[i][j] = 0;
				}
			}	
		}
}



void VoteMatrix::PE (int V, int K, double p) //Polya-Eggenberger generation idea 
{
	int N = this->numrows();
	int m = this->numcols();
//	_m.resize(N, vector<int>(m,0));
	int i = 0;
	int i2 = 0;
	int j = 0;
	double r = 0;
	int t = this->numvoters();
	
	//choose the random generators
	/*
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine gen(seed); // try mersenne twister
   	uniform_int_distribution<int> dis(0,N-1);
	uniform_real_distribution<double> dis2(0.0,1.0);
*/

	for (int k = 0; k < (V-t)/K; k++) 
	{
		i = Random::dis(0,N-1); //choose a district
		i2 = i;
		r = Random::dis2(); // decide whether to imitate own or another district
		if (r <=p) 
		{
			while(i==i2) {
				i2 = Random::dis(0,N-1); //rejection sampling, go until get another district					
			}		
		}		
		else
		{
			
		} // now we have found the district to imitate
		// now choose the voter to imitate
		//** should compute cdf and do choosing together!
		//** utility function to choose from disc dist
		
		vector<int>c = vector<int>(m+1,0); //cumulative sum
		int v = 0;
		vector<double>C = vector<double>(m+1,0); //cdf
		
		for (int j = 0; j < m; j++) // compute cdf
		{
			c[j+1] = c[j] + _m[i2][j];
			v = v + _m[i2][j];	
		}
		
		for (int j = 0; j < m+1; j++) 
		{
			C[j] = (double)c[j]/(double)v;
		}
		
		r = Random::dis2(); //random colour in the district
		j = 0;
		while(r > C[j] && j < m+1) {
			j++;
		} //now j is the right colour to imitate - should use binary search!
		_m[i][j-1] = _m[i][j-1] + K;	
	}
}

void VoteMatrix::swing(vector<double> newpoll, string t) 
{
	int N = _m.size();
	int m = _m[0].size(); // what to do if m=0?
	int i,j,c;
	vector<double> natswing(m,0);
	vector<double> oldvote(m,0);
	vector<double> oldvotesq(m,0);
	vector<double> rowtot(N,0);
	double temp = 0;
	
	VoteMatrix M = *this;
	VoteMatrix M2 = M;
	//cout << "original matrix" << endl;
	//M.print();
	M2.aggregate(N);
//	cout << "original popular vote" << endl;
	//M2.print();
	int n = M.numvoters();
	cout << "Number of districts: " << N << endl;
	
	for (int i = 0; i < N; i++) 
	{
		for (int j = 0; j < m; j++) 
		{
			rowtot[i]+=_m[i][j];
		}
	}

	
	for (int j = 0; j < m; j++) 
		{
			oldvote[j] = (double)M2.get(0,j)/n; // need to check for division by zero
			natswing[j] = newpoll[j] - oldvote[j]; // the national swing 
			for (int i = 0; i < N; i++) {
				oldvotesq[j]+= (double)M.get(i,j)*M.get(i,j)/(rowtot[i]*rowtot[i]);
			}
			oldvotesq[j] = oldvotesq[j]/N;
		//	cout << "j = " << j << "oldvote[j] = " << oldvote[j] << "oldvotesq[j] = " << oldvotesq[j] << endl;
		}	
	
	
	
	// need to check for bad input
	
	

	for(i = 0; i < N; i++) 
	{
		
		for (j = 0; j < m; j++) 
		{
			
			temp = (double)_m[i][j]/rowtot[i]; // old vote share in district i 
			 if(t == "UNS") // "Uniform National Swing"
			 {
			 	_m[i][j] = round(rowtot[i]*(temp + natswing[j]));			 	
			 }
			 else if (t == "PLH") //"Proportional Loss Hypothesis" 	adjusted for mean
			 {
				 _m[i][j] = round(rowtot[i]*(temp + natswing[j]*temp/oldvote[j]));
			 }
			 else if (t=="PLM") //piecewise linear model
			 {
			 	if (natswing[j] < 0)
			 		{
					 	_m[i][j] = round(rowtot[i]*(temp + natswing[j]*temp/oldvote[j]));
					}
				else
					{
					_m[i][j] = round(rowtot[i]*(temp + natswing[j]*(1-temp)/(1-oldvote[j]))); // division by zero possible
					}
			 }
			 
			 else if (t=="Q") // "Quadratic model"
			 {
			 
			  _m[i][j] = round(rowtot[i]*(temp + natswing[j]*temp*(1-temp)/(oldvote[j] - oldvotesq[j])));	
			 }
		}
		
	}
	
	//cout << "Swung matrix" << endl;
	//(*this).print();	
	//M2 = *this;
	//M2.aggregate(N);
	//cout <<"Swung popular vote"<<endl;
	//M2.print();

}


// functions

double VoteMatrix::frob(VoteMatrix M2) // need to check same dimensions, defn may also be weird
{
		double t = 0;
		for(int i = 0; i < this->numrows(); i++) 
		{
			for (int j = 0; j < this->numcols(); j++) 
			{
				t+=	(_m[i][j] - M2.get(i,j))*(_m[i][j] - M2.get(i,j));	
			}		
		}
		return 2*sqrt(t)/((double)this->numvoters()+(double)M2.numvoters());
}

void VoteMatrix::print() const { //use extra print function?
	for (int i = 0; i < _m.size(); i++) 
	{
		cout << _m[i][0];
		for (int j = 1; j < _m[0].size(); j++) 
		{
			cout << " " << _m[i][j];
		}
	cout << endl;
	}
}
	
vector<double> VoteMatrix::poll() {
	int N = _m.size();
	int m = _m[0].size();
	vector<double> v = vector<double>();
	
	int c = 0;
	int c2 = 0;
	for (int j = 0; j < m; j++) {
		c = 0;
		for (int i = 0; i < N; i++) {
			c+=_m[i][j];
			c2+=_m[i][j];		
		}
		v.push_back(c);
	}
	
	for (int j = 0; j < m; j++) {
		v[j] = v[j]/c2;	
	}
	
	
	return v;
}


