#include <iostream>

#include "matrix.hpp"

using namespace std;
using namespace matrix;

int main() {
	try {
		Matrix input;
		cout << "A = ";
		cin >> input;

		cout << "A =" << endl << input << endl;
	} catch (invalid_argument& e) {
		cerr << "Error: " << e.what() << endl;
	}
	return 0;
}
