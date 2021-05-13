/*******************************************************************************
 * Name        : sieve.cpp
 * Author      : Sanjana Madhu
 * Date        : September 18, 2020
 * Description : Sieve of Eratosthenes
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

class PrimesSieve {
public:
    PrimesSieve(int limit);

    ~PrimesSieve() {
        delete [] is_prime_;
    }

    int num_primes() const {
        return num_primes_;
    }

    void display_primes() const;

private:
    // Instance variables
    bool * const is_prime_;
    const int limit_;
    int num_primes_, max_prime_;



    // Method declarations
    int count_num_primes() const;
    void sieve();
    static int num_digits(int num);
};

PrimesSieve::PrimesSieve(int limit) :
        is_prime_{new bool[limit + 1]}, limit_{limit} {
    sieve();
}

void PrimesSieve::display_primes() const {


	const int max_prime_width = num_digits(max_prime_); //finds the number of digits of largest prime number
	const int primes_per_row = 80 / (max_prime_width + 1); //finds the number of prime numbers allowed per row

	//Special case where the limit is 2. Finds that the number of primes is 1.
	if(limit_ == 2){
		cout << endl;
		cout << "Number of primes found: " << 1 << endl;
		cout << "Primes up to " << limit_ << ":" << endl;
		cout << limit_ << endl;
	}

	else {
		int primes_counter = primes_per_row; //counter that decrements when a prime number is printed

		//When the number of prime numbers requires wrapping to the next line
		if (num_primes_ > primes_per_row) {
			cout << endl;
			cout << "Number of primes found: " << num_primes_ << endl;
			cout  << "Primes up to " << limit_ << ":" << endl;
			for (int i = 2; i <= limit_; i++){
				if(primes_counter > 0){
					if(is_prime_[i] == true) {
						//Ensures that a space is NOT printed after the last prime number
						if (i == max_prime_ || primes_counter == 1) {
							cout << setw(max_prime_width) << i;
							primes_counter--;
						}
						else {
							cout << setw(max_prime_width) << i << " ";
							primes_counter--;
						}

					}
				}
				//if the number of primes is decremented to zero, then reset the primes_counter back to primes_per_row
				if(primes_counter == 0){
					cout << endl;
					primes_counter = primes_per_row;
				}
			}
		}

		//When the number of prime numbers can fit on a single line
		if (num_primes_ <= primes_per_row) {
			cout << endl;
			cout << "Number of primes found: " << num_primes_ << endl;
			cout  << "Primes up to " << limit_ << ":" << endl;
			for (int i = 0; i <= limit_ ; i++){
				if(is_prime_[i] == true) {
					//Ensures that a space is NOT printed after the last prime number
					if (i == max_prime_) {
						cout << i;
					}
					else {
						cout << i << " ";
					}
				}
			}
		}

	}

}



int PrimesSieve::count_num_primes() const {
    //Count the number of primes found
	int counter = 0;
	for (int i = 0; i <= limit_; i++){
		if(is_prime_[i] == true){
			counter++; //counter is incremented when a prime number is found
		}
	}
    return counter;
}

void PrimesSieve::sieve() {
    //Sieve algorithm

	//Set 0 and 1 equal to false; it is known that they can never be prime numbers
	is_prime_[0] = false;
	is_prime_[1] = false;

	//Initializes the boolean array by setting each element eqaul to true
	for(int i = 2; i <= limit_; i++){
		is_prime_[i] = true;
	}

	//iterates through the boolean array to find prime values
	for(int i = 0; i <= sqrt(limit_); i++){
		if (is_prime_[i] == true){
			int newi = i*i; //created to ensure that all multiples of i are set to false
			for (int j = newi; j <= limit_; j += i) {
				is_prime_[j] = false;
			}
		}
	}

	//iterates through the boolean array and constantly resets the value of max_prime until the limit is reached
	for(int i = 0; i <= limit_; i++){
		if(is_prime_[i] == true){
			max_prime_ = i;
		}
	}

	//sets the number of primes to be the value returned from count_num_primes();
	num_primes_ = count_num_primes();


}

int PrimesSieve::num_digits(int num) {
    //Determines how many digits are in an integer. Counts the number of times that a given number can be divided by 10.
	int counter = 0;

	while (num > 0){
		num = num / 10;
		counter++;
	}
    return counter;
}

int main() {
    cout << "**************************** " <<  "Sieve of Eratosthenes" <<
            " ****************************" << endl;
    cout << "Search for primes up to: ";
    string limit_str;
    cin >> limit_str;
    int limit;

    // Use stringstream for conversion. Don't forget to #include <sstream>
    istringstream iss(limit_str);

    // Check for error.
    if ( !(iss >> limit) ) {
        cerr << "Error: Input is not an integer." << endl;
        return 1;
    }
    if (limit < 2) {
        cerr << "Error: Input must be an integer >= 2." << endl;
        return 1;
    }

    //Uses class to produce the desired output.
    PrimesSieve sieve(limit);
    sieve.display_primes();

    return 0;
}
