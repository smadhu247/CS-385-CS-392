/*******************************************************************************
 * Name        : stairclimber.cpp
 * Author      : Sanjana Madhu
 * Date        : October 2, 2020
 * Description : Lists the number of ways to climb n stairs.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;

vector< vector<int> > get_ways(int num_stairs) {
    // Returns a vector of vectors of ints representing the different combinations of ways to climb num_stairs
    // stairs, moving up either 1, 2, or 3 stairs at a time.

	//uses a combination of the first and second approach as seen in the python code
	vector<vector<int>> ways;
	if (num_stairs <= 0){ //utilizes first approach to just returns empty vector for the base case
		return {{}};
	}
	else {
		for(int i = 1; i < 4; i++){
			vector<vector<int>> result;
			if (num_stairs >= i){
				result = get_ways(num_stairs - i);
				int length = result.size();
				for(int j = 0; j < length; j++) {
					//uses the second approach using "insert" in c++
					result[j].insert(result[j].begin(), i); //(iterator position, const value_type& val)
				}
				ways.insert(ways.end(), result.begin(), result.end()); //(iterator position, InputIterator first, InputIterator last)
			}
		}
	}
	return ways;
}

void display_ways(const vector< vector<int> > &ways) {
    //Displays the ways to climb stairs by iterating over the vector of vectors and printing each combination.

	int digit_count = 0; //holds max number of digits to set proper width
	int count_size = ways.size();
	while (count_size > 0){ //counts the number of digits in the max number
		digit_count++;
		count_size = count_size / 10;
	}

	int number_count = 1; //stores value to number every respective "way"
	int size = ways.size();
	for(int i = 0; i < size; i++){
		cout << setw(digit_count) << number_count << ". " << "["; //prints number and sets width of row
		int len = ways[i].size();
		for(int j = 0; j < len; j++){
			if (j <= len - 2){
				cout << ways[i][j] << ", "; //prints all ways except for the last element in each way
			}
			else {
				cout << ways[i][j]; //prints last element in each way
			}
		}
		cout << "]" << endl;
		number_count++;
	}
}

int main(int argc, char * const argv[]) {

	istringstream iss;

	//checks that arguments are given
	if(argc != 2){
		cerr << "Usage: " << argv[0] << " <number of stairs>" << endl;
		return 1;
	}

	//takes an input value of number of stairs
	int input;
	iss.str(argv[1]);
	iss >> input;

	//checks that input is neither a negative number not a string
	if(input <= 0){
		cerr << "Error: Number of stairs must be a positive integer." << endl;
		return 1;
	}

	vector<vector<int>> ways = get_ways(input); //calls and stores possible ways in 2d vector

	if(input == 1){
		cout << "1 way to climb 1 stair." << endl;
	}
	else {
		cout <<  ways.size() << " ways to climb " << input << " stairs." << endl;
	}

	display_ways(ways);
	return 0;

}
