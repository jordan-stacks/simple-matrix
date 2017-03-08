#include <iostream>

#include "matrix.hpp"

using namespace std;
using namespace matrix;

int test_inverse(void) {
	try {
		Matrix M, MI;

		M = Matrix{3, 3, {3,0,2,2,0,-2,0,1,1}};
		MI = M.invert();
		if (!(M * MI == identityMatrix(3)))
			return false;

		return true;
	} catch(exception& e) {
		cerr << e.what() << endl;
		return false;
	}
}
