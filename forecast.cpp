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
#include "measures.h"
#include "random.h"
#include "extra.h"
#include "simulate.h"
#include "forecast.h"

void Forecast::forecast(string oldfile, string newfile)
{	
	
		VoteMatrix M = VoteMatrix(oldfile);	
		VoteMatrix M2 = M;
		VoteMatrix M3 = M;
		VoteMatrix M4 = M;
		VoteMatrix M5 = M;
		
		VoteMatrix M6 = VoteMatrix(newfile);
		VoteMatrix M7 = M6;
		Parliament p,p1;
		
		vector<double>natswing;
		
		
		cout << "Forecasting from " << oldfile << " to " << newfile << endl;

		cout << "Real results under FPP for base election" << endl;
		p = Rules::FPP(M);
		p.print();
		
		cout << "Real results under FPP for new election" << endl;
		p = Rules::FPP(M6);
		p.print();	
		
		
		// work out scaling for swing models
		int Nold = M.numvoters();
		int Nnew = M6.numvoters();
		M7.aggregate(M7.numrows());
		
		vector<double> poll = vector<double>();
		for (int j = 0; j < M7.numcols(); j++) 
		{
			poll.push_back(M7.getrow(0)[j]/(double)Nnew);
		}
			
		cout << "New popular vote fractions" << endl;
		Extra::print(poll);
		cout << "New popular vote totals" << endl;
		M7.print();
	
		M.aggregate(M.numrows());
		vector<double>old = vector<double>();
		for (int j = 0; j < M.numcols(); j++) 
		{
			old.push_back(M.getrow(0)[j]/(double)Nold);
		}
		cout << "Old popular vote" << endl;
		Extra::print(old);
		cout << "Original number of votes: " << Nold << endl;
		
		cout << "National swing " << endl;
		for (int j = 0; j < M.numcols(); j++) 
		{
			natswing.push_back(poll[j] - old[j]);
		}
		Extra::print(natswing);
					
					
		// swing model predictions and errors
		cout << "Predictions under FPP using multiplicative swing model" << endl;
		M3.swing(poll, "PLH"); 
		M3.print();
		p1 = Rules::FPP(M3);
		p1.print();
		cout << "L2 distance: " << p.l2dist(p1)<< endl;
		
		cout << "Predictions under FPP using additive swing model" << endl;
		M4.swing(poll, "UNS"); 
		M4.print();
		p1 = Rules::FPP(M4);
		p1.print();
		cout << "L2 distance: " << p.l2dist(p1) <<endl;
		
		cout << "Predictions under FPP using piecewise swing model" << endl;
		M5.swing(poll , "PLM"); 
		M5.print();
		p1 = Rules::FPP(M5);
		p1.print();
		cout << "L2 distance: " << p.l2dist(p1) <<endl;
		
}

void Forecast::forecast(string oldfile, vector<double> poll, string t, int f, int d, int c)
{	
	
		VoteMatrix M = VoteMatrix(oldfile);	
		VoteMatrix M2 = M;
		VoteMatrix M3 = M;
		VoteMatrix M4 = M;
		VoteMatrix M5 = M;
		
		Parliament p,p1;
		
		int V = M.numvoters()/f; //what fraction of voters to use (if convergence is quick)
		
		vector<double>natswing;
		
		cout << "Forecasting from base election " << oldfile << endl;
		cout << "Real results under FPP for base election" << endl;
		p = Rules::FPP(M);
		p.print();
		
		cout << "Poll estimate given by user" << endl;
		Extra::print(poll);
		
		
		// work out scaling for swing models
		int Nold = M.numvoters();
		M.aggregate(M.numrows());
		vector<double>old = vector<double>();
		for (int j = 0; j < M.numcols(); j++) 
		{
			old.push_back(M.getrow(0)[j]/(double)Nold);
		}
		//cout << "Old popular vote" << endl;
		//Extra::print(old);
		
		cout << "National swing based on poll estimate " << endl;
		for (int j = 0; j < M.numcols(); j++) 
		{
			natswing.push_back(poll[j] - old[j]);
		}
		Extra::print(natswing);
					
							
		// swing model predictions and errors
		cout << "Point prediction under FPP using crude swing model " << t << endl;
		M2.swing(poll, t); 
		p1 = Rules::FPP(M2);
		p1.print();
		//cout << "L2 distance between real and predicted parliaments " << p.l2dist(p1)<< endl;
		
		// more refined probabilistic results
		
		M3.percentmatrix(d); //downscaled version of original votematrix
		
		int win = 0;
		int sum = 0;

	Random::setseed(99);	
	
	cout << "Predictions under FPP using swing, downscale, PE model" << endl;
	for (int i = 0; i < c; i++)
	{
		M4 = M2;
		M4.percentmatrix(d);
		M4.PE(V, 1, 0.08);
		p = Rules::FPP(M4);
		p.print();				
	}
		
}

