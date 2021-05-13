/*******************************************************************************
 * Name    : sqrt.cpp
 * Author  : Sanjana Madhu
 * Version : 1.0
 * Date    : September 3, 2020
 * Description : Finds the square root of a number
 * Pledge : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <stdlib.h>
#include <limits>
#include <iomanip>
#include <iostream>
#include <iostream>
#include <sstream>

using namespace std;

//Finds the square root of a number using euler's method
double sqrt(double num, double epsilon){


	double last_guess = num;
	double next_guess = (last_guess + num/last_guess)/ 2;


	if (num < 0){
		return numeric_limits<double>::quiet_NaN();
	}

	if(num == 0 || num == 1){
		return num;
	}

	//case where square root is consistently guessed
	while (abs(last_guess - next_guess) > epsilon){
		last_guess = next_guess;
		next_guess = (last_guess + num/last_guess)/ 2;
	}

	return next_guess;
}

int main (int argc, char *argv[]){

	double a;
	double b;

	b = 1e-7;

	istringstream iss;

	if(argc != 2 && argc != 3) {
		cerr << "Usage: " << argv[0] << " <value> [epsilon]" << endl;
		return 1;
	}

	iss.str(argv[1]);
	if (!(iss >> a)){
		cerr << "Error: Value argument must be a double." << endl;
		return 1;
	}

	if (argc == 3){
	iss.clear();
	iss.str(argv[2]);
		if (!(iss >> b) || b <= 0){
			cerr << "Error: Epsilon argument must be a positive double." << endl;
			return 1;
		}
	}

	cout << fixed << setprecision(8) << sqrt(a,b) << endl;
	return 0;
}



