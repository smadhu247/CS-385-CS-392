/*******************************************************************************
 * Name        : inversioncounter.cpp
 * Author      : Sanjana Madhu
 * Version     : 1.0
 * Date        : 10/30/20
 * Description : Counts the number of inversions in an array.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cctype>
#include <cstring>

using namespace std;

// Function prototype.
static long mergesort(int array[], int scratch[], int low, int high);

/**
 * Counts the number of inversions in an array in theta(n^2) time.
 */
long count_inversions_slow(int array[], int length) {
    long inv = 0;
    for(int i = 0; i < length; i++){
    	for(int j = i + 1; j < length; j++){
    		if(array[i] > array[j]){
    			inv++;
    		}
    	}
    }
	return inv;
}

/**
 * Counts the number of inversions in an array in theta(n lg n) time.
 */
long count_inversions_fast(int array[], int length) {
	int* scratch_array = new int[length];
	long inv = mergesort(array, scratch_array, 0, length-1); //calls mergesort; low is index 0, high is index length - 1
	delete[] scratch_array;
	return inv;
}

static long mergesort(int array[], int scratch[], int low, int high) {
	long inv = 0;
	int mid = 0;
	if (low < high){
		mid = low + (high - low) / 2;
		inv = inv + mergesort(array, scratch, low, mid); //inversions are counted when mergesort is called on both sides of array
		inv = inv + mergesort(array, scratch, mid+1, high);
		int L = low;
		int H = mid + 1;
		for (int i = low; i <= high; i++){
			if(L <= mid && (H > high || array[L] <= array[H])){
				scratch[i] = array[L];
				L = L + 1;
			}
			else{
				inv = inv + mid - L + 1; //increase number of inversions in this case
				scratch[i] = array[H];
				H = H + 1;
			}
		}
		for(int i = low; i <= high; i++){
			array[i] = scratch[i]; //rewrites array from scratch
		}
	}
	return inv;
}


int main(int argc, char *argv[]) {
    //parse command-line argument

	//if the string at argv[1] != "slow"
	if(argc == 2){
		if(strcmp("slow", argv[1]) != 0){
			cerr << "Error: Unrecognized option '" << argv[1] << "'." << endl;
			return 1;
		}
	}
	//too many arguments, throw usage error
	if(argc > 2){
		cerr << "Usage: " << argv[0] << " [slow]" << endl;
		return 1;
	}

    cout << "Enter sequence of integers, each followed by a space: " << flush;

    istringstream iss;
    int value, index = 0;
    vector<int> values;
    string str;
    str.reserve(11);
    char c;
    while (true) {
        c = getchar();
        const bool eoln = c == '\r' || c == '\n';
        if (isspace(c) || eoln) {
            if (str.length() > 0) {
                iss.str(str);
                if (iss >> value) {
                    values.push_back(value);
                } else {
                    cerr << "Error: Non-integer value '" << str
                         << "' received at index " << index << "." << endl;
                    return 1;
                }
                iss.clear();
                ++index;
            }
            if (eoln) {
                break;
            }
            str.clear();
        } else {
            str += c;
        }
    }

    int length = values.size();

    //if no values are inputed, error
    if(length == 0){
		cerr << "Error: Sequence of integers not received." << endl;
		return 1;
	}
    //If argc = 2, we are using the slow method
	if(argc == 2) {
		cout << "Number of inversions: " << count_inversions_slow(&values[0], length) << endl;
	}
	//If argc = 1, we are using the fast method
	if(argc == 1) {
		cout << "Number of inversions: " << count_inversions_fast(&values[0], length) << endl;
	}

	return 0;
 }
