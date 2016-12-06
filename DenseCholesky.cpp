//DenseCholesky.cpp
//g++ -I /Users/ShuoFan/Documents/eigen DenseCholesky.cpp -o dense

#include <iostream>
#include <fstream>
#include <Eigen/Dense>
#include <cmath>
#include <ctime>
using namespace Eigen;
using namespace std;

void cholesky(int n, MatrixXd& A)    //right looking cholesky
{
	for (int i=0;i<n;i++)
	{
		A(i,i)=sqrt(A(i,i));
		for(int j=i+1;j<n;j++)
			A(j,i)/=A(i,i);
		if(i<n-1)
		{
			for(int j=i+1;j<n;j++)
			{
				for (int k=i+1;k<n;k++)
					A(j,k)-=A(j,i)*A(k,i);
			}
		}
	}
	for (int i=0;i<n;i++)
		for (int j=i+1;j<n;j++)
			A(i,j)=0;
}

int main(int argc, char *argv[])
{
	int nx,ny,num;
	ifstream fin(argv[1]);
	while (fin.peek() == '%')fin.ignore(2048, '\n');
	fin>>nx>>ny>>num;
	int *row = new int[num];
	int *column = new int[num];
	MatrixXd A=MatrixXd::Zero(nx+1,ny+1);
	for(int i=0;i<num;i++)
		fin>>row[i]>>column[i]>>A(row[i]-1,column[i]-1);
//	int N=3;
//	MatrixXd A(N,N);
//	A<<4,12,-16,12,37,-43,-16,-43,98;
	cout<<A<<endl;
	clock_t begin = clock();
	cholesky(nx,A);
	printf("Run time: %.6fs\n", (double)(clock() - begin) / CLOCKS_PER_SEC);
	cout<<A<<endl;
	cout<<A*A.transpose()<<endl;
	delete[] row;
	delete[] column;
	return 0;
}