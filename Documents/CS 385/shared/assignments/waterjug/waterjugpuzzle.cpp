/*******************************************************************************
 * Name        : waterjugpuzzle.cpp
 * Author      : Sanjana Madhu, Lasya Josyula
 * Date        : October 19, 2020
 * Description : Water jug puzzle
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/

#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;
// Struct to represent state of water in the jugs.

struct State {
	int a, b, c;

	State *parent;
	State(int _a, int _b, int _c) :
		a{_a}, b{_b}, c{_c}, parent{nullptr} { }
		// String representation of state in tuple form.

		string to_string() {
			ostringstream oss;
			oss << "(" << a << ", " << b << ", " << c << ")";
			return oss.str();
		}

		//used in checking if the current state has already been visited.
		bool is_equal(State* otherState){
			if(a == otherState->a && b == otherState->b && c == otherState->c){
				return true;
			}
			return false;
		}
};

/*
 * we only commented out one of the sections.
 * the rest follows the same logic.
 */
void displaySolution (vector<State> solutionVector){
	//print the initial state
	cout << "Initial state. " << solutionVector[0].to_string() << endl;

	//loops through our solution vector
	for (unsigned int i = 1; i < solutionVector.size(); i++){

		//checks if the jug A received water
		if (solutionVector[i].a > solutionVector[i-1].a){
			//checks how much water is being poured
			int diff = solutionVector[i].a - solutionVector[i-1].a;
			//checks if jug B poured it
			if (solutionVector[i].b < solutionVector[i-1].b){
				//if only one gallon poured
				if(diff == 1){
					cout << "Pour " << diff << " gallon from B to A. " << solutionVector[i].to_string() << endl;
				}
				else{
					cout << "Pour " << diff << " gallons from B to A. " << solutionVector[i].to_string() << endl;
				}
			}
			//checks if the jug C poured it
			if (solutionVector[i].c < solutionVector[i-1].c){
				//if only one gallon poured
				if(diff == 1){
					cout << "Pour " << diff << " gallon from C to A. " << solutionVector[i].to_string() << endl;
				}
				else{
					cout << "Pour " << diff << " gallons from C to A. " << solutionVector[i].to_string() << endl;
				}
			}
		}
		if (solutionVector[i].b > solutionVector[i-1].b){
			int diff = solutionVector[i].b - solutionVector[i-1].b;
			if (solutionVector[i].a < solutionVector[i-1].a){
				if( diff == 1){
					cout << "Pour " << diff << " gallon from A to B. " << solutionVector[i].to_string() << endl;
				}
				else{
					cout << "Pour " << diff << " gallons from A to B. " << solutionVector[i].to_string() << endl;
				}
			}
			if (solutionVector[i].c < solutionVector[i-1].c){
				if(diff == 1){
					cout << "Pour " << diff << " gallon from C to B. " << solutionVector[i].to_string() << endl;
				}
				else{
					cout << "Pour " << diff << " gallons from C to B. " << solutionVector[i].to_string() << endl;
				}

			}
		}
		if (solutionVector[i].c > solutionVector[i-1].c){
			int diff = solutionVector[i].c - solutionVector[i-1].c;
			if (solutionVector[i].b < solutionVector[i-1].b){
				if(diff == 1){
					cout << "Pour " << diff << " gallon from B to C. " << solutionVector[i].to_string() << endl;
				}
				else{
					cout << "Pour " << diff << " gallons from B to C. " << solutionVector[i].to_string() << endl;
				}

			}
			if (solutionVector[i].a < solutionVector[i-1].a){
				if(diff == 1){
					cout << "Pour " << diff << " gallon from A to C. " << solutionVector[i].to_string() << endl;
				}
				else{
					cout << "Pour " << diff << " gallons from A to C. " << solutionVector[i].to_string() << endl;
				}

			}
		}
	}
}

/*
 * We only commented out the pour C to A if statement.
 * The other five if statements work with the same logic.
 */
void bfs(int cap_a, int cap_b, int cap_c, State* goal){
	queue<State*> queue;
	vector<State*> visited = {};

	//initializes the initial state with 0, 0, and the capacity of jug c
	State* initial = new State(0, 0, cap_c);
	//pushes the initial state to the queue and adds it to visited
	queue.push(initial);
	visited.insert(visited.begin(), initial);

	int waterA;
	int waterB;
	int waterC;

	//bfs only occurs while there is at least one node in the queue.
	while(queue.size() > 0) {
		waterA = queue.front()->a;
		waterB = queue.front()->b;
		waterC = queue.front()->c;

		//checks if the current values match the goal values
		//adds this node to the final vector and breaks out of the while
		if(waterA == goal->a && waterB == goal->b &&waterC == goal->c){
			vector<State> final = {};
			State* temp = queue.front();
			for (unsigned int i = 0; i < visited.size(); i++){
				final.insert(final.begin(), *temp);
				if (temp->parent == nullptr){
					break;
				}

				temp = temp->parent;
			}
			//calls on display solution with the final solution vector
			displaySolution(final);
			break;
		}

		// Pour from C to A
		if(waterA < cap_a && waterC > 0){

			//creates dummy node to be added to the queue
			State* newNode = new State(0,0,0);

			//checks if the space in the "to" jug is smaller or the current value in the "from" jug is smaller
			//pours the minimum value between the two.
			int diff = min(waterC, cap_a-waterA);

			//sets the a, b, and c values to the adjusted values
			newNode->a = waterA + diff;
			newNode->b = waterB;
			newNode->c = waterC - diff;

			//sets the parent of the dummy to the node in the front of the queue
			newNode->parent = queue.front();

			// checks if the state was already visited, if so, break out.
			bool broken = true;
			for(unsigned int i = 0; i < visited.size(); i++){
				if(newNode->is_equal(visited[i])){
					broken = false;
					break;
				}
			}
			//if the state was not visited, push the dummy node into the queue
			//insert current state into the visited vector, and put the this state into the matrix
			if(broken){
				queue.push(newNode);
				visited.insert(visited.begin(), newNode);
			}
			//otherwise delete the newNode immediately
			else{
				delete newNode;
			}

		}

		// Pour from B to A
		if(waterA < cap_a && waterC > 0){
			State* newNode = new State(0,0,0);
			int diff = min(waterB, cap_a-waterA);
			newNode->a = waterA + diff;
			newNode->b = waterB - diff;
			newNode->c = waterC;
			newNode->parent = queue.front();
			bool broken = true;
			for(unsigned int i = 0; i < visited.size(); i++){
				if(newNode->is_equal(visited[i])){
					broken = false;
					break;
				}
			}

			if(broken){
				queue.push(newNode);
				visited.insert(visited.begin(), newNode);
			}
			else{
				delete newNode;
			}
		}

		// Pour from C to B
		if(waterB < cap_b && waterC > 0){
			State* newNode = new State(0,0,0);
			int diff = min(waterC, cap_b-waterB);
			newNode->a = waterA;
			newNode->b = waterB + diff;
			newNode->c = waterC - diff;
			newNode->parent = queue.front();
			bool broken = true;
			for(unsigned int i = 0; i < visited.size(); i++){
				if(newNode->is_equal(visited[i])){
					broken = false;
					break;
				}
			}

			if(broken){
				queue.push(newNode);
				visited.insert(visited.begin(), newNode);
			}
			else{
				delete newNode;
			}
		}

		// Pour from A to B
		if(waterB < cap_b && waterC > 0){
			State* newNode = new State(0,0,0);
			int diff = min(waterA, cap_b-waterB);
			newNode->b = waterB + diff;
			newNode->a = waterA - diff;
			newNode->c = waterC;
			newNode->parent = queue.front();
			bool broken = true;
			for(unsigned int i = 0; i < visited.size(); i++){
				if(newNode->is_equal(visited[i])){
					broken = false;
					break;
				}
			}

			if(broken){
				queue.push(newNode);
				visited.insert(visited.begin(), newNode);
			}
			else{
				delete newNode;
			}
		}

		// Pour from B to C
		if(waterC < cap_c && waterC > 0){
			State* newNode = new State(0,0,0);
			int diff = min(waterB, cap_c-waterC);
			newNode->a = waterA;
			newNode->c = waterC + diff;
			newNode->b = waterB - diff;
			newNode->parent = queue.front();
			bool broken = true;
			for(unsigned int i = 0; i < visited.size(); i++){
				if(newNode->is_equal(visited[i])){
					broken = false;
					break;
				}
			}

			if(broken){
				queue.push(newNode);
				visited.insert(visited.begin(), newNode);
			}
			else{
				delete newNode;
			}
		}

		// Pour from A to C
		if(waterC < cap_c && waterC > 0){
			State* newNode = new State(0,0,0);
			int diff = min(waterA, cap_c-waterC);
			newNode->c = waterC + diff;
			newNode->a = waterA - diff;
			newNode->b = waterB;
			newNode->parent = queue.front();
			//need a way to push stuff into queue or else infinite loop
			bool broken = true;
			for(unsigned int i = 0; i < visited.size(); i++){
				if(newNode->is_equal(visited[i])){
					broken = false;
					break;
				}

			}

			if(broken){
				queue.push(newNode);
				visited.insert(visited.begin(), newNode);
			}
			else{
				delete newNode;
			}
		}
		//pops the first element of the queue.
		queue.pop();

		//prints no solution if we've exhausted every possible option.
		if(queue.empty()){
			cout << "No solution." << endl;
		}

	}

	//deletes the states in visited
	for(unsigned int i = 0; i< visited.size(); i++){
		delete visited[i];
	}

	//clears the queue and visited.
	while(!queue.empty()){
		queue.pop();
	}
	visited.clear();
}


int main(int argc, char *argv[]) {
	//values:
	// 0 	   1 	  2       3     4      5
	//cap A, cap B, cap C, goal A, goal B, goal C

	//argc:
	//	1 	  2       3     4      5 	    6
	//cap A, cap B, cap C, goal A, goal B, goal C
	if(argc != 7){
		cerr << "Usage: ./waterjugpuzzle <cap A> <cap B> <cap C> <goal A> <goal B> <goal C>" << endl;
		return 1;
	}

	int values[6];
	for (int i = 0; i < argc - 1; i++){
		istringstream iss(argv[i+1]);
		iss >> values[i];
	}

	if (values[2] <= 0){
		cerr << "Error: Invalid capacity '" << argv[3] << "' for jug C." << endl;
		return 1;
	}

	for (int i = 1; i < argc; i++) {
		istringstream iss(argv[i]);
		int value;
		string goal;
		string capacity;

		if (!(iss >> value) || value < 0){
			if(i == 1 || i == 4){
				capacity = "A";
				goal = "A";
			}
			if(i == 2 || i == 5){
				capacity = "B";
				goal = "B";
			}
			if(i == 6){
				//capacity = "C";
				goal = "C";
			}

			if(i < 3){
				cerr << "Error: Invalid capacity '" << argv[i] << "' for jug " << capacity << "." << endl;
				return 1;
			}
			if(i >= 3){
				cerr << "Error: Invalid goal '" << argv[i] << "' for jug " << goal << "." << endl;
				return 1;
			}

		}
	}

	if(values[3] > values[0]){
		cerr << "Error: Goal cannot exceed capacity of jug A." << endl;
		return 1;
	}

	if(values[4] > values[1]){
		cerr << "Error: Goal cannot exceed capacity of jug B." << endl;
		return 1;
	}

	if(values[5] > values[2]){
		cerr << "Error: Goal cannot exceed capacity of jug C." << endl;
		return 1;
	}

	int sum = values[3] + values[4] + values[5];
	if(values[2] != sum){
		cerr << "Error: Total gallons in goal state must be equal to the capacity of jug C." << endl;
		return 1;
	}

	int a = values[0];
	int b = values[1];
	int c = values[2];
	int d = values[3];
	int e = values[4];
	int f = values[5];
	State* goal = new State(d, e, f);
	bfs(a,b,c, goal);

	delete goal;
	return 0;

}

