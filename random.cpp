#include<random>
#include "random.h"


unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
default_random_engine gen(seed);

int Random::dis(int a, int b) {

	return uniform_int_distribution<int>(a,b)(gen);

}

double Random::dis2() {

	return uniform_real_distribution<double>(0,1)(gen);
}

void Random::setseed(unsigned int s) {

	gen.seed(s);
}

