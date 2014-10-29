#include <stdio.h>
#include <omp.h>
#include <mpi.h>

static int threads_cnt = 4;

static double the_func(double x)
{
	return (x - 3) * (x - 1) * (x + 1) / 5;
}

static double rectangel_method(double start, double dx, long long int startIndex, long long int endIndex) {
	double sum = 0;
	double curX;
#pragma omp parallel for num_threads (threads_cnt) reduction (+:sum) private (curX)
	printf("%lld - %lld\n", startIndex, endIndex);
	for (long long int i = startIndex; i <= endIndex; ++i) {
		curX = start + i * dx;
		sum += the_func(curX);
	}
	return sum;
}

int main(int argc, char* argv[]) {
	double ans = 0;

	double start = -3;
	double end = 4;
	long long int gran = 10000000000; //10000M
	double dx = (end - start) / gran;	

	long long int startIndex, endIndex;
	int rank, size;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	long long int myGran = gran / size;
	long long int commonMod = gran % size;

	int sd = commonMod > rank ? rank : commonMod;
	startIndex = myGran * rank + sd;

	int fd = commonMod > rank ? 1 : 0;
	endIndex = startIndex + myGran + fd - 1;

	double res = rectangel_method(start, dx, startIndex, endIndex);
	//printf("%.9lf\n", res);
	
	MPI_Reduce(&res, &ans, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	
	if(rank == 0)
		printf("Final result is %.12lf.\n", ans * dx);
	
	MPI_Finalize();
	

	return 0;
}
