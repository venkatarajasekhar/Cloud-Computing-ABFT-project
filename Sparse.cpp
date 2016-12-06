//
//

#include <fstream>
#include <cstdlib>
#include "Matrix.h"
using namespace std;

int main(int argc, char *argv[])
{
	matrix A(argv[1]);
	A.converthalfCSC();
	A.generateetree();
	A.lfactorstructure();
	A.cholesky();
	return 0;
}