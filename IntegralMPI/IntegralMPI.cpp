#define _USE_MATH_DEFINES

#include <mpi.h>
#include <cmath>
#include <math.h>
#include <stdio.h>
#include <iostream>
using namespace std;
double f(double x) {
	return sin(x);
}
double Trapeze(double A, double B, int n, int rank, int size) {
	double a = (B - A) / size * rank;
	//cout << "rank= " << rank << " a= " << a << " ";
	double b = (B - A) / size * (rank + 1);
	//cout << "b= " << b << " ";
	double h = (b - a) / n;
	//cout << "h= " << h << " ";
	double sum = f(a) + f(b);
	for (int i = 1; i <= n; i++) {
		sum += 2 * f(a + i * h);
	}
	sum *= h / 2;
	//cout << "integral= " << sum << endl;
	return sum;
}
int main(int argc, char* argv[])
{
	double a, b;
	long double Result, sum;
	int rank, size;
	double startwtime = 0.0;
	double endwtime;
	int n, i, source, dest=0, tag=123;
	a = 0;
	b = M_PI_2;
	n = std::pow(10, 7);
	MPI_Status status;
	// initializes the mpi execution enviroment
	MPI_Init(&argc, &argv);
	//get this process' rank (proccess within a communicator)
	//MPI_COMM_WORLD is the default communicator
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	// get the total number ranks in this communicator
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (rank == 0)
	{
		startwtime = MPI_Wtime();
	}

	cout << "Hello, MPI! Rank: " << rank << " size " << size << endl;
	//cout << "Trapeze " << Trapeze(a, b, n/size, rank, size) << endl;
	sum = Trapeze(a, b, n / size, rank, size);

	if (rank == 0)
	{
		Result = sum;
		for (i = 1; i < size; i++) {
			MPI_Recv(&sum, 1, MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			Result += sum;
		}

		cout << "Result=" << Result << endl;
		endwtime = MPI_Wtime();
		cout << "time= " << (endwtime - startwtime) * 1000 << endl;
	}
	else {
		MPI_Send(&sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}
	//cout << "Result=" << Result << endl;

	//cout << "Trapeze " << Trapeze(a, b, n, rank, size) << endl;
	//Terminate MPI execution enviroment
	MPI_Finalize();
	return 0;
}