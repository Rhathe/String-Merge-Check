#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

using namespace std;

int TheMatrix[1002][1002] = {0};	//Double Array for DYNAMIC PROGRAMMING!
									//Rows correspond to the first string, columns correspond to the second
									//First, or 0th, row and column are always 0
									//And strings, for resons later, add an extra character in front,
									//Hence the two extra rows/columns

//Moves current position down and to the left, basically diagonally, and returns 0
//Unless it reaches the end of the diagonal line, in which case it moves to the next diagonal line and returns 1
//Which makes the name stepItUp admittedly confusing
int stepItUp(int &i, int &j, int &previ, int &prevj, int width, int height) {
	if ( (i <= 1) || (j >= height)) {
		i = previ + 1;
		j = prevj;
		
		if ( i > width ) {
			i = width;
			++j;
		}
		
		previ = i;
		prevj = j;
		
		return 1;
	}

	else {
		--i;
		++j;
		
		return 0;
	}
}

//Checks if the cell position above was a valid path, if so,  it returns a 1
//If not and the cell position to the left was a valid path, it returns a 2
//Otherwise it returns a 0
int checkLeftUp(int i, int j) {
	if ( TheMatrix[i-1][j] == (i-1) + j - 1 )
		return 1;
	else if ( TheMatrix[i][j - 1] == i + (j-1) - 1 )
		return 2;
	else
		return 0;
}

//Takes the mixed string, and uppercases the letters according to the array
//Checks vertically at first, corresponding to the second string, and if valid, will keep lowercase and move to next letter
//Otherwise, it will uppercase
void backtrackJack(int i, int j, string &s) {
	for (int k = s.size() - 1; k >= 0; --k) {
		if (TheMatrix[i][j-1] == i + (j-1) - 1) {
			--j;
		}
		else {
			--i;
			s[k] += 'A' - 'a';
		}
	}
}

//Actual "checking if merged" function
int scrabble(string &s1, string &s2, string &mix) {
	
	//If it's not the right size to be a merge, stop and return 1
	if (mix.size() != s1.size() + s2.size())
		return 1;
	
	//Necessary filler in the beginning, to add a row and column of "not this string yet"
	s1.insert(0, "\n");
	s2.insert(0, "\n");
	
	int width = s1.size();
	int height = s2.size();
	
	//Begin at position (2,1), prev variables to determine where to start next diagonal
	int i = 2;
	int previ = i;
	int j = 1;
	int prevj = j;
	
	bool onLikeDonkeyKong = false;	//variable to determine if check should stop after a diagonal is checked
									//If there is no valid path after through a diagonal, then stop
	
	//Loop through 2-D array
	while(1) {
		//check if going to this cell from above is a valid path, if so, mark cell and set check variable to true
		//Reason for [i + j - 3]: Because of the default row and column of 0's, it's actually (i-1) and (j-1)
		//Then, because of those extra new line characters, you subtract an extra 1
		//Thus, [(i-1) + (j-1) - 1] = [i + j -3]
		if ((checkLeftUp(i,j) == 2) && (s2[j-1] == mix[i + j - 3]) ) {
			TheMatrix[i][j] = i + j - 1;
			onLikeDonkeyKong = true;
		}
		//check if going to this cell from the left is a valid path, if so, mark cell and set check variable to true
		else if ((checkLeftUp(i,j) == 1) && (s1[i-1] == mix[i + j - 3])) {
			TheMatrix[i][j] = i + j - 1;
			onLikeDonkeyKong = true;
		}
		//Else, mark cell as 0
		else
			TheMatrix[i][j] = 0;
		
		//Check if still inside array, i.e. not last cell
		//If diagonal had a valid path, continue onto next diagonal, else return 1
		//If last cell, break
		if ( i + j < width + height ) {
			if (stepItUp(i,j,previ,prevj,width,height) == 1) {
				if (onLikeDonkeyKong == true)
					onLikeDonkeyKong = false;
				else
					return 1;
			}	
		}
		else
			break;
	}
	
	//If last cell was a valid path, then merge was valid and backtrack to output correct string
	if (onLikeDonkeyKong == true) {
		backtrackJack(i,j,mix);
		return 0;
	}	
	else
		return 1;
}

int main() {
	TheMatrix[1][1] = 1;	//Necessary default
	
	ifstream input;
	ofstream output;

	string s1, s2, s3, dummy;
	
	cout << "Enter name of input file: ";
	cin >> dummy;
	input.open(dummy.c_str());
	
	//error check to see if input can be opened
	if(!input.is_open()) {
		cerr << "DANGER DANGER DANGER WILL ROBINSON\n";
		exit(1);
	}
	
	cout << "Enter name of output file: ";
	cin >> dummy;
	output.open(dummy.c_str());
	
	//Loop through file, checking merge status of strings
	while(1) {
		input >> s1;
		input >> s2;
		
		if (input.eof())
			break;
		
		input >> s3;
		
		//Check merge, if not valid, output message
		if (scrabble(s1,s2,s3) == 1)
			s3 = "*** NOT A MERGE ***";
	
		output << s3 << endl;
	}
	
	
	input.close();
	output.close();
}
