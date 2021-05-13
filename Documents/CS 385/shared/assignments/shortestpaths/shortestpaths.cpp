/*******************************************************************************
 * Name        : shortestpaths.cpp
 * Author      : Sanjana Madhu, Lasya Joysula
 * Date        : 12/7/2020
 * Description : Floyd's algorithm.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>
#include <iterator>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <locale>
using namespace std;

long inf = numeric_limits<long>::max(); //representation of infinity
int num_vertices = 0;
long** matrix_distance;
long** matrix_paths;
long** matrix_intermediate;
string alphabet[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N",
		"O", "P", "Q", "R", "S", "T", "U", "V", "W", "X" "Y", "Z"};

/*
 * Pseudocode for Floyd Algorithm:
 * Alg floyd (W[1...n, 1....n])
 * D(0) = W
 * for k, 1 to n
 * 		for j, 1 to n
 * 			for i, 1 to n
 * 				D(k-1)[i,j] = min (D(k-1)[i,j], D(k-1)[i,k] + D(k-1)[k,j]
 * 	return D(n)
 *
 */

/*Floyd algorithm
 * Updates the path matrices with optimized weightings
 * Updates intermediate matrix to show the path from one vertex to another
 */
long** floyd(long** matrix){
	for(int k = 0; k < num_vertices; k++){
		for(int j = 0; j < num_vertices; j++){
			for (int i = 0; i < num_vertices; i++){
				if (matrix[i][k] != inf && matrix[k][j] != inf){
					long dist = min (matrix[i][j], matrix[i][k] + matrix[k][j]);
					if (matrix[i][k] + matrix[k][j] < matrix[i][j]){
						matrix[i][j] = dist;
						matrix_intermediate[i][j] = k;
					}
				}
			}
		}
	}
	return matrix;
}

/*
 * Recursive function that creates the solution tree
 * Recurses on the left and right children of the tree.
 * Appends only the left child of leaf nodes to the solution vector
 * 		    DC
 * 	   	   / \
 * 	   	 DA   AC
 * Here, D and A will be in the vector that is returned by path_checker
 */
vector<char> path_checker(long x, long y){
	vector<char> final;
	long inter_dist = matrix_intermediate[x][y];
	if (matrix_intermediate[x][y] != inf){
		vector<char> left_child = path_checker(x, inter_dist);
		vector<char> right_child = path_checker(inter_dist, y);
		//concatenates left and right child vectors
		left_child.insert(left_child.end(), right_child.begin(), right_child.end());
		return left_child;
	}
	//casts the string vector to a char
	char val = alphabet[x].c_str()[0];
	final.insert(final.end(), val);
	return final;
}


/*
 * Displays the paths of every vertex to every other vertex.
 * Displays conditions where the row and column are equal, when the weight is not infinity, and when the weight is infinity
 */
void display_paths() {
	vector<char> paths;
	for(int i = 0; i < num_vertices; i++){
		for(int j = 0; j < num_vertices; j++){
			if(i == j){
				cout << alphabet[i] << " -> " << alphabet[j] << ", distance: " << matrix_paths[i][j] << ", path: " << alphabet[i] << endl;
			}
			else if (matrix_paths[i][j] != inf){
				if (matrix_intermediate[i][j] != inf){
					cout << alphabet[i] << " -> " << alphabet[j] << ", distance: " << matrix_paths[i][j] << ", path: ";
					paths = path_checker(i, j);
					for (unsigned int k = 0; k < paths.size(); k++){
						cout << paths[k] << " -> ";
					}
					cout << alphabet[j]  << endl;
				}
				else{
					cout << alphabet[i] << " -> " << alphabet[j] << ", distance: " << matrix_paths[i][j] << ", path: " << alphabet[i] << " -> " << alphabet[j] << endl;
				}
			}
			else {
				cout << alphabet[i] << " -> " << alphabet[j] << ", distance: infinity, path: none" << endl;
			}
		}
	}
}


/**
 * Displays the matrix on the screen formatted as a table.
 */
void display_table(long** const matrix, const string &label, const bool use_letters = false) {
	cout << label << endl;
	long max_val = 0;
	for (int i = 0; i < num_vertices; i++) {
		for (int j = 0; j < num_vertices; j++) {
			long cell = matrix[i][j];
			if (cell < inf && cell > max_val) {
				max_val = matrix[i][j];
			}
		}
	}
	int max_cell_width = use_letters ? to_string(max_val).length() : to_string(max(static_cast<long>(num_vertices), max_val)).length();
	cout << ' ';
	for (int j = 0; j < num_vertices; j++) {
		cout << setw(max_cell_width + 1) << static_cast<char>(j + 'A');
	}
	cout << endl;
	for (int i = 0; i < num_vertices; i++) {
		cout << static_cast<char>(i + 'A');
		for (int j = 0; j < num_vertices; j++) {
			cout << " " << setw(max_cell_width);
			if (matrix[i][j] == inf ) {
				cout << "-";
			} else if (use_letters) {
				cout << static_cast<char>(matrix[i][j] + 'A');
			} else {
				cout << matrix[i][j];
			}
		}
		cout << endl;
	}
	cout << endl;
}

int main(int argc, char *argv[]) {

	if (argc != 2){
		cerr << "Usage: ./shortestpaths <filename>" << endl;
		return 1;
	}
	//source for reading files: https://stackoverflow.com/questions/7868936/read-file-line-by-line-using-ifstream-in-c
	string file_name = argv[1];
	ifstream infile(file_name);
	string line;

	if(argc == 2 && !infile.is_open()) {
		cerr << "Error: Cannot open file '" << file_name << "'." << endl;
		return 1;
	}

	int num; //holds value of the first integer read in the file

	if (infile.is_open()){
		int line_number = 1;
		getline(infile, line);
		istringstream iss(line);
		iss >> num;
		num_vertices = num;

		if (num < 1 || num > 26){
			cerr << "Error: Invalid number of vertices '" << line << "' on line " << line_number << "." << endl;
			return 1;
		}

		//initializes the distance matrix with infinities and zeros on the diagonals
		matrix_distance = new long*[num_vertices];
		for (int i = 0; i < num_vertices; i++) {
			matrix_distance[i] = new long[num_vertices];
			fill(matrix_distance[i], num_vertices + matrix_distance[i], inf);
			matrix_distance[i][i] = 0;
		}

		while (getline(infile, line)) {
			 line_number++;
			 istringstream iss(line);
			 string word;
			 vector<string> values;

			 //values will contain every term on one line
			 while(iss >> word) {
				 values.push_back(word);
			 }

			 if(values.size() != 3){
				 cerr <<"Error: Invalid edge data '" << line << "' on line " << line_number << "." << endl;
				 return 1;
			 }

			 if (!isdigit(values[2][0])){
				 cerr << "Error: Invalid edge weight '" << values[2] << "' on line " << line_number << "." << endl;
				 return 1;
			 }

			 if(values[0].length() > 1 || values[1].length() > 1){
				 if(values[0].length() > 1){
					 cerr << "Error: Starting vertex '" << values[0] << "' on line " << line_number <<
							 " is not among valid values " << alphabet[0] << "-" << alphabet[num - 1] << "." << endl;
					 return 1;
				 }
				 if(values[1].length() > 1){
					 cerr << "Error: Ending vertex '" << values[1] << "' on line " << line_number <<
							 " is not among valid values " << alphabet[0] << "-" << alphabet[num - 1] << "." << endl;
					 return 1;
				 }
			 }

			 if ((stoi(values[2])) <= 0){
				 cerr << "Error: Invalid edge weight '" << values[2] << "' on line " << line_number << "." << endl;
				 return 1;
			 }

			 else {
				 if(values[0] > alphabet[num - 1] || values[0] < "A"){
					 cerr << "Error: Starting vertex '" << values[0] << "' on line " << line_number <<
							 " is not among valid values " <<  alphabet[0] << "-" << alphabet[num - 1] << "." << endl;
					 return 1;
				 }
				 if(values[1] > alphabet[num - 1] || values[1] < "A"){
					 cerr << "Error: Ending vertex '" << values[1] << "' on line " << line_number <<
							 " is not among valid values " <<  alphabet[0] << "-" << alphabet[num - 1] << "." << endl;
					 return 1;
				 }
			}
			//inserts the weights into the distance matrix
			char from = values[0][0];
			char to = values[1][0];
			int weight = stoi(values[2]);
			matrix_distance[from - 'A'][to - 'A'] = weight;
		}
	}
	infile.close();

	//intermediate matrix initialized with infinities
	matrix_intermediate =  new long*[num_vertices];
	for(int i = 0; i < num_vertices; i++) {
		matrix_intermediate[i] = new long[num_vertices];
		fill_n(matrix_intermediate[i], num_vertices, inf);
	}

	//path matrix is initialized with the values in the distance matrix
	matrix_paths =  new long*[num_vertices];
	for(int i = 0; i < num_vertices; i++) {
		matrix_paths[i] = new long[num_vertices];
		for(int j = 0; j < num_vertices; j++) {
			matrix_paths[i][j] = matrix_distance[i][j];
		}
	}

	matrix_paths = floyd(matrix_paths);
	display_table(matrix_distance, "Distance matrix:", false);
	display_table(matrix_paths, "Path lengths:", false);
	display_table(matrix_intermediate, "Intermediate vertices:", true);
	display_paths();

	//deletes matrices off of the heap
	for (int i = 0; i < num_vertices; i++) {
		delete[] matrix_distance[i];
		delete[] matrix_paths[i];
		delete[] matrix_intermediate[i];
	}
	delete[] matrix_distance;
	delete[] matrix_paths;
	delete[] matrix_intermediate;
	return 0;
}
