/*******************************************************************************
 * Name        : unique.cpp
 * Author      : Sanjana Madhu
 * Date        : September 25, 2020
 * Description : Determining uniqueness of chars with int as bit vector.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <cctype>
#include <iostream>
#include <sstream>

using namespace std;

bool is_all_lowercase(const string &s) {
    // returns true if all characters in string are lowercase letters in the English alphabet; false otherwise

	int length = s.length();
	for (int i = 0; i < length; i++) {
		if (s[i] < 'a' || s[i] > 'z'){ //uses ASCII values to check if character in string is not lowercase
			return false;
		}
	}
	return true;
}

bool all_unique_letters(const string &s) {
    // returns true if all letters in string are unique, that is no duplicates are found; false otherwise.
    // You may use only a single int for storage and work with bitwise and bitshifting operators.
    // No credit will be given for other solutions.

	unsigned int vector = 0; //holds values for each letter. 1 if already seen, 0 if not seen yet
	int setter = 0; //has value of 1 in the bit corresponding to the letter's order in the alphabet
	int length = s.length();
	for (int i = 0; i < length; i++){
		setter = 1 << (s[i] - 'a');
		if ((vector & setter) != 0){ //if bitwise and operation is != 0, then a letter has been repeated
			return false;
		}
		else {
			vector = (vector | setter); //resets vector to incorporate a 1 at the bit corresponding to the new letter found
		}
	}
	return true;
}

int main(int argc, char * const argv[]) {
    // reads and parses command line arguments.
    // Calls other functions to produce correct output.

		if(argc == 1) {
			cerr << "Usage: " << argv[0] << " <string>" << endl;
			return 1;
		}

		if(argc > 2) {
			cerr << "Usage: " << argv[0] << " <string>" << endl;
			return 1;
		}

		if(is_all_lowercase(argv[1]) == false){
			cerr << "Error: String must contain only lowercase letters." << endl;
			return 1;
		}

		if(is_all_lowercase(argv[1]) == true && all_unique_letters(argv[1]) == true){
			cout << "All letters are unique." << endl;
		}

		if(is_all_lowercase(argv[1]) == true && all_unique_letters(argv[1]) == false){
			cout << "Duplicate letters found." << endl;
		}

		return 0;

}
