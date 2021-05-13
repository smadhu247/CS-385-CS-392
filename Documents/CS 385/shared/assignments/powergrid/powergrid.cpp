/*******************************************************************************
 * Name        : powergrid.cpp
 * Author      : Sanjana Madhu
 * Date        : 12/18/2020
 * Description : Powergrid created with Kruskal's algorithm.
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
#include <stdlib.h>
#include <queue>
using namespace std;

int num_vertices = 0;
long** matrix;
long inf = numeric_limits<long>::max(); //representation of infinity
vector<int> parent;

/* Struct created for each street
 * contains street length, street name, and the two vertices
 */
struct Edge {
	int a, b;
	int length;
	string street;
	Edge(int _a, int _b, int length, string _street) :
		a{_a}, b{_b}, length{length}, street{_street} { }
};

vector<Edge*> MST_edges; //holds edges in MST
vector<Edge*> total_edges; //hold all edges in the graph

/*
 * Find's the parent vertex of each vertex in the graph
 *
 */
int find_parent(long vert){
	for(unsigned int i = 0; i < parent.size(); i++){
		if (vert != parent[vert]){
			vert = parent[vert];
		}
	}
	return vert;
}

int kruskal(long** graph) {
    int min_total = 0; //contains total length of the MST
    int edge_counter = 0; //keeps track of the number of inspected edges
    long length = inf; //hold current edge's length
    long current_len= 0;
    string street;

    //finds an the edge and length to be placed in MST
    while (edge_counter < num_vertices - 1) {
    	length = inf;
        long vert_one = -1;
        long vert_two = -1;
        for (int i = 0; i < num_vertices; i++) {
            for (int j = 0; j < num_vertices; j++) {
                if (graph[i][j] != inf && find_parent(i) != find_parent(j)) {
                	if (graph[i][j] < length){
                		vert_one = i;
                		vert_two = j;
                		length = graph[i][j];
                		current_len = length;
                	}
                }
            }
        }

        //if the vertices don't change, then we have reached our no solution case
        if (vert_one == -1 || vert_two == -1){
			return -1;
		}

        //creates a union between two edges. in the end, all vertices in MST will point to one vertex
        int parent_vert_one = find_parent(vert_one);
        int parent_vert_two = find_parent(vert_two);
        parent[parent_vert_one] = parent_vert_two;

        //increment edge_counter and update the total length of MST
        edge_counter++;
        min_total += length;

        //finds the correct street to be placed into Edge Struct
        for (unsigned int i = 0; i < total_edges.size(); i++){
        	if(vert_one + 1 == total_edges[i]->a && vert_two + 1 == total_edges[i]->b && current_len == total_edges[i]->length){
        		street = total_edges[i]->street;
        	}
        }

        //creates a new edge and places into the vector of edges in the MST
        Edge* edge = new Edge(vert_one, vert_two, current_len, street);
        MST_edges.push_back(edge);
    }
    return min_total;
}

/*
 * Displays the MST if there exists a valid MST in the graph
 */
void display_solution(int total_length){
	vector<string> streets;
	for (unsigned int i = 0; i < MST_edges.size(); i++){
		streets.push_back(MST_edges[i]->street);
	}
	sort(streets.begin(), streets.end()); //sorts streets alphabetically
	cout << "Total wire length (meters): " << total_length << endl;
	for (unsigned int i = 0; i < streets.size(); i++){
		for (int j = MST_edges.size() - 1; j >= 0; j--){
			if(streets[i] == MST_edges[j]->street){
				cout << MST_edges[j]->street << " [" << MST_edges[j]->length << "]" << endl;
			}
		}
	}
}

/*
 * Checks whether a string is a valid number or not
 * Used for error checking in the main function
 */
bool is_number(string s){
	int len = s.length();
	for(int i = 0; i < len; i++) {
		if(!(s[i] >= '0' && s[i] <= '9')){
			return false;
		}
	}
	return true;
}

int main(int argc, char *argv[]) {

	if (argc != 2){
		cerr << "Usage: ./powergrid <input file>" << endl;
		return 1;
	}

	//reads files
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
		istringstream iss2(line);
		string check_num;
		iss2 >> check_num;
		bool check = is_number(check_num);

		if (!(iss >> num)|| num < 1 || num > 1000 || !check){
			cerr << "Error: Invalid number of vertices '" << line << "' on line " << line_number << "." << endl;
			return 1;
		}

		num_vertices = num;

		//initializes matrix that will correspond vertices to their lengths
		matrix = new long*[num_vertices];
		for (int i = 0; i < num_vertices; i++) {
			matrix[i] = new long[num_vertices];
			fill(matrix[i], num_vertices + matrix[i], inf);
		}


		while (getline(infile, line)) {
			 line_number++;
			 istringstream iss(line);
			 string word;
			 vector<string> values;

			 while (iss.good()) { //geeksforgeeks: https://www.geeksforgeeks.org/program-to-parse-a-comma-separated-string-in-c/
				 string substr;
				 getline(iss, substr, ',');
				 values.push_back(substr);
			 }

			 if(values.size() != 4){
				 cerr <<"Error: Invalid edge data '" << line << "' on line " << line_number << "." << endl;
				 return 1;
			 }


			 bool check_one = is_number(values[0]);
			 bool check_two = is_number(values[1]);
			 bool check_three = is_number(values[2]);

			 if(!check_one || stoi(values[0]) > num || stoi(values[0]) == 0){
				 cerr << "Error: Starting vertex '" << values[0] << "' on line " << line_number <<
						 " is not among valid values " << 1 << "-" << num << "." << endl;
				 return 1;
			 }

			 if(!check_two || stoi(values[1]) > num || stoi(values[1]) == 0){
				 cerr << "Error: Ending vertex '" << values[1] << "' on line " << line_number <<
						 " is not among valid values " << 1 << "-" << num << "." << endl;
				 return 1;
			 }

			 if(!check_three || stoi(values[2]) <= 0 ){
				 cerr << "Error: Invalid edge weight '" << values[2] << "' on line " << line_number << "." << endl;
				 return 1;
			 }

			//stores values into the matrix
			int from = stoi(values[0]);
			int to = stoi(values[1]);
			int length = stoi(values[2]);
			matrix[from - 1][to - 1] = length;
			string street = values[3];
			Edge* pair = new Edge(from, to, length, street);
			total_edges.push_back(pair); //inserts edge into a vector containing all edges in the graph
		}
		infile.close();
	}

	//initializes parent vector
	for (int i = 0; i < num_vertices; i++) {
		parent.push_back(i);
	}

	//performs Kruskal's to find the MST
	int min_tree = kruskal(matrix);

	//if -1 is returned by the function, then there is no solution found
	//if there is only one vertex, then there are no edges in the graph, so the size is 0
	if (min_tree == -1 || total_edges.size() == 0){
		cout << "No solution." << endl;
	}
	//if not, the function that displays the solution is called
	else{
		display_solution(min_tree);
	}

	//deletes values on the heap
	for(unsigned int i = 0; i < total_edges.size(); i++){
		delete total_edges[i];
	}
	for(unsigned int i = 0; i < MST_edges.size(); i++){
		delete MST_edges[i];
	}
	for (int i = 0; i < num_vertices; i++) {
		delete[] matrix[i];
	}
	delete[] matrix;
	return 0;
}
