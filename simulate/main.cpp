#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <random>
#include <array>
#include <cmath>
using namespace std;
#include "votematrix.h"
#include "profile.h"
#include "parliament.h"
#include "rules.h"
#include "measures.h"
#include "extra.h"
#include "simulate.h"
#include "forecast.h"
#include "random.h"

using namespace Rules;
using namespace Measures;
using namespace Extra;
using namespace Simulate;
using namespace Forecast;

void datagen (int N, int m, vector<vector<int> > I, double p=0.0, int V=1000000, int K=1) 
{

 ostringstream filename;
 filename << "seatvote"<< N<<"."<<m<<"."<<p;
 ofstream file(filename.str());

ostringstream filename2;
 filename2 << "voteshare"<<N<<"."<<m<<"."<<p;
 ofstream file2(filename2.str());


for (int k = 0; k < 1000; k++) 
{

 	VoteMatrix M = VoteMatrix(N,m);
 	for (int i = 0; i < N; i++) 
 	{
   		for (int j = 0; j < m; j++) 
     	M.set(i,j,I[i][j]);
 	}
 	M.PE(V, K, p);
 	Parliament P = FPP(M);
  
	for (int i = 0; i < N; i++)
	{
		file2 << M.get(i,0) <<" ";	
	}
	file2 << endl;
	
	file << P.get(0);
 	M.aggregate(M.numrows());
 	file <<" "<< M.get(0,0) <<endl;
}
 
file.close();
file2.close();
}

void datagen2 (int N, int m, vector<vector<int> > I, double p=0.0, int V=1000000, int K=1) 
{

 ostringstream filename;
 filename << "districtvote"<< N<<"."<<m<<"."<<p;
 ofstream file(filename.str());




 	VoteMatrix M = VoteMatrix(N,m);
 	for (int i = 0; i < N; i++) 
 	{
   		for (int j = 0; j < m; j++) 
     	M.set(i,j,I[i][j]);
 	}
 	M.PE(V, K, p);
 	Parliament P = FPP(M);
  
  
 	for (int k = 0; k < M.numrows(); k++)
 	{
 		file << M.get(k,0) << " " << M.get(k,1) <<endl;
 	}

 
file.close();
}

void swingtest (int N, int m, double p = 0.0)
{
	VoteMatrix M, M1, M2;
	double s;
	int oldvote, newvote;
	ostringstream filename2;
	filename2 << "swingtest"<<N<<"."<<m<<"."<<p;
	ofstream file2(filename2.str());


	for (int k = 0; k < 1000; k++)
  	{
  		M = VoteMatrix(N,m);
 		for (int i = 0; i < N; i++) 
 		{
   			for (int j = 0; j < m; j++) 
     			M.set(i,j,1);
 		}
  		M.PE(N*N*N,1,p);
  		M1 = M;
  		M.percentmatrix(6*N);
  		M.PE(N*N*N,1,p);  
  		
  		oldvote = M1.get(0,0);
  		newvote = M.get(0,0);
  		
  		file2 << (double)newvote/M.numvoters(0) << " " << (double)oldvote/M1.numvoters(0) << " ";
		M.aggregate(N);	
		M1.aggregate(N);
		s = (double)M.get(0,0)/M.numvoters() - (double)M1.get(0,0)/M1.numvoters();
		file2 << s;
		file2 << endl;
			
	
  	
  	}
  	
}
  	
  	
int main(int argc, char* argv[])
{

/*

// ********************* election forecasting 


 	string file = "../datasets/Wales2017trim.csv"; 
 	string oldfile = "../datasets/Wales2015trim.csv"; 
	
	//forecast("../datasets/Scotland2015trim.csv", file);

	VoteMatrix M(file);
	Parliament p;
	vector<double> realpoll = M.poll();
	VoteMatrix M2(oldfile);
	//vector<double> prepoll{0.33,0.49,0.045,0.02,0.0,0.0,0.10,0,0,0,0,0,0.035}; syntax problem
	
	//Extra::print(realpoll);
	//Extra::print(prepoll);
	
	cout << "Real results under FPP for later election" << endl;
	p = Rules::FPP(M);
	p.print();	
	
	cout << "Retrospective results for later election" << endl;	
	forecast(file, realpoll, "PLH", 20, 50, 100);
	cout << "Predicted results for later election using exit poll" << endl;
	forecast(oldfile, realpoll, "PLH", 20, 50, 100);
//	cout << "Predicted results for later election using opinion poll" << endl;
//	forecast(oldfile, prepoll, "PLH", 20, 50, 100);
	//forecast(oldfile, prepoll, "UNS", 20, 50, 100);
	
*/

/*
// testing swing models

	string newfile = "../datasets/Wales2017trim.csv"; 
 	string oldfile = "../datasets/Wales2015trim.csv"; 
	forecast(oldfile, newfile);
	
	// BIG PROBLEM WITH OVERFLOW

*/	

 
 
 // ********************** simulation of electoral systems ************************
 
    // Check the number of parameters
    if (argc < 5) {
        // Tell the user how to run the program
        std::cerr << "Usage: " << argv[0] << " simulation type" << " N" << " m" << " n" << std::endl;
        return 1;
    }
      
    vector<vector<double> > v;
  	switch (*argv[1])
  		{
  		case 's': 
  		v = simulate_SM(atoi(argv[2]), atoi(argv[3]), atoi(argv[4])); break;
   		case 't':
   		v = simulate_threshold(atoi(argv[2]), atoi(argv[3]), atoi(argv[4])); break;
   		case 'a':
   		v = simulate_alloc(atoi(argv[2]), atoi(argv[3]), atoi(argv[4])); break;
   		case 'p':
   		v = simulate_simple(atoi(argv[2]), atoi(argv[3]), atoi(argv[4])); break;
   		//case 'f': not working yet
   		//Forecast::forecast();
  		}
  	Extra::print(v);
  	

 	


/*
// *********************** testing PE generator
  	
	swingtest(100,2,0.0);
	swingtest(100,2,0.1);
	swingtest(100,2,0.2);
*/


/*
  	
// ******************** generating artificial plurality data   	

 	
  	if(argc < 4)
  	{
  	  cerr << "Usage: " << argv[0] << " (number of districts) (number of parties) (out-of-district imitation parameter)\n";
  	  return 1;
  	}
  	
  	int N = atoi(argv[1]);
  	int m = atoi(argv[2]);
  	double p = atof(argv[3]);
  	
  	vector<vector<int> > I(N,vector<int>(m,1)); // all parties with 1 voter in each district initially
  	
  	
  	
  // asymmetric initial conditions
  
  
  	for (int i = 0; i < 8*N/10; i++) 
  	{
  		I[i][0] = 0;
  		I[i][1] = 2;
  		I[i][2] = 2;
  	}
  	for (int i = 8*N/10; i < 9*N/10; i++) 
  	{
  		I[i][0] = 1;
  		I[i][1] = 2;
  		I[i][2] = 2;
  	}
  	for (int i = 9*N/10; i < N; i++) 
  	{
  		I[i][0] = 3;
  		I[i][1] = 1;
  		I[i][2] = 1;
  	}
  	// alternative asymmetric initial conditions
  	for (int i = 1; i< N; i++) 
  	{
  	I[i][1]=2;
  	I[i][2]=2;
  	}
  
  
  	// exchangeable, biased in all districts
  	
  	
  	for (int i = 0; i< N/2; i++) 
  	{
  	I[i][0]=5;
  	}
  	for (int i = N/2; i< N; i++) 
  	{
  	I[i][1]=2;
  	}
  	
  	
  	
  	
  	// exchangeable, symmetric in all districts
  	
  	for(int i = 0; i < N/2; i++) 
  	{
  	I[i][0]=2;
  	I[i][1]=2;
  	}
  	
  	
  	//datagen2(N, m, I, p, N*N*N, 1);
  	
  	
 	
 	
 	
*/


// ******************** retrospective simulation 


/*
	//string file = "../datasets/Canada2015/Canada2015_district_trim.csv"; 
	string file = "../datasets/Wales2017trim.csv";
	
	VoteMatrix M(file);
	VoteMatrix M2;
	VoteMatrix M3;
	Parliament p;
	int V = M.numvoters()/30; // save time, may need to revisit this, and change PE to give different district sizes
	p = FPP(M);
	cout << "Original election result: " << endl;
	p.print();
	
	//for (int i = 0; i < M.numrows(); i++) 
	//{
	//	cout << i << " ";
	//	print(M.getrow(i));
	//}
	
	
	//untweaked
	
	int win = 0;
	int sum = 0;
	int c = 100;
	
	Random::setseed(99);
	
	vector<int> distwins(M.numrows(), 0); // first estimate prob of winning each district
	cout << "Counterfactual election results: " << endl;
	
	M.percentmatrix(50);
	
	for (int j = 0; j < c; j++)
		{
		M2 = M;		
		M2.PE(V, 1, 0.08);
		p = FPP(M2);
		p.print();
		for (int i = 0; i < distwins.size(); i++) 
			{
				if (Rules::winner(M2.getrow(i)) == 0) 
				{
					distwins[i]++;
				}		
			}				
		}
		
	// tweaked 
	int oldc = c;
	c = 100;
	

	
	cout << "Tweaked counterfactual election results: " << endl;
	
	for (int w = 1; w <= oldc; w+=2) 
	{
	cout << "width = " << w << endl;
	M2 = M;

	int b = 240;
	while (b > 0) //distribute extra voters
		{
		for (int i = 0; i < M2.numrows(); i++) 
			{
			if ((2*distwins[i] >= oldc - w) && (2*distwins[i] <= oldc + w) && (b > 0) )
				{
				M2.set(i, 0, M2.get(i, 0) + 1);
				b--;
				}
			}
		}
		
	Random::setseed(99);		
	for (int j = 0; j < c; j++)
		{			
		//cout <<  "Voters distributed" << endl;
		//M2.print();
		M3 = M2;
		M3.PE(V, 1, 0.08);
		p = FPP(M3);		
		p.print();
		vector<int> distwins2(M.numrows(), 0);
		
		for (int i = 0; i < M.numrows(); i++)
			{
			if (Rules::winner(M3.getrow(i)) == 0) 
				{
				distwins2[i]++;
				}
			}
			
		for (int i = 0; i < M.numrows(); i++) 
			{
			//cout << i << " " << distwins[i] << " " << distwins2[i] << endl;
			}
		
					
		}		
	
	}
		
*/					

	


	
	
	

 
  	
	return 0;	
	
	
	
	
	
}	
