#include <iostream>

#include "matrix.h"

using namespace std;
using namespace simple_matrix;

int test_cofactor(void) {
	matrix M;

	M = matrix(3, 3, {1,4,7,3,0,5,-1,9,11});
	if (M.cofactor(1, 2) != -13)
		return false;

	return true;
}
