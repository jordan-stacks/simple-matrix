#include <iostream>

#include "matrix.h"

using namespace std;
using namespace simple_matrix;

int test_inverse(void) {
	matrix M, MI;

	M = matrix(3, 3, {3,0,2,2,0,-2,0,1,1});
	MI = M.invert();
	if (!(M * MI == identity_matrix(3)))
		return false;

	return true;
}
