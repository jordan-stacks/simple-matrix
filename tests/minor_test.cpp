#include <iostream>

#include "matrix.h"

using namespace std;
using namespace simple_matrix;

int test_minor(void) {
	matrix M;

	M = matrix(4, 4, {1,2,3,4,2,2,2,4,4,5,6,4,7,8,9,4});
	if (M.minordet(1, 3) != 0)
		return false;

	M = matrix(4, 4, {1,3,-2,1,5,1,0,-1,0,1,0,-2,2,-1,0,3});
	if (M.minordet(0, 2) != 3)
		return false;

	return true;
}
