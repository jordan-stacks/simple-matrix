#include <iostream>
#include <fstream>

#include "matrix.h"

using namespace std;
using namespace simple_matrix;

int test_determinant(void) {
	matrix M;
	int det, comp;
	
	ifstream in("tests/determinants.txt");
	
	while (true) {
		in >> M;
		if (M.is_empty())
			break;
		in >> det;
		comp = M.det();
		if (int(comp) != det) {
			cout << endl << M.pretty() << endl;
			cout << "Det should be " << det << endl;
			cout << "computed " << comp << endl;
			return false;
		}
	}

	return true;
}
