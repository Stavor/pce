#include <stdio.h>
#include <omp.h>
#include <mpi.h>

static int processes_cnt = 16;
static int threads_cnt = 8;

static double the_func(double x)
{
	return (x - 3) * (x - 1) * (x + 1) / 5;
}

static double rectangel_method(double start, double dx, int startIndex, int endIndex) {
	double sum = 0;
	double curX;
#pragma omp parallel for num_threads (threads_cnt) reduction (+:sum) private (curX)
	printf("%d - %d\n", startIndex, endIndex);
	for (int i = startIndex; i <= endIndex; ++i) {
		curX = start + i * dx;
		sum += the_func(curX);
	}
	return sum * dx;
}

int main(int argc, char* argv[]) {
	double start = -3;
	double end = 4;
	int gran = 100000000; //100M
	double dx = (end - start) / gran;	

	int startIndex, endIndex;
	int rank, size;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	int myGran = gran / size;
	int commonMod = gran % size;

	int sd = commonMod > rank ? rank : commonMod;
	startIndex = myGran * rank + sd;

	int fd = commonMod > rank ? 1 : 0;
	int isLast = size - 1 == rank ? 2 : 0;
	endIndex = startIndex + myGran + fd - 1 + isLast;

	double res = rectangel_method(start, dx, startIndex, endIndex);
	printf("%.9lf\n", res);
	
	MPI_Finalize();
	
	return 0;
}
