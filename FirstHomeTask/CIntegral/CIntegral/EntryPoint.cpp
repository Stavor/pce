#include <stdio.h>
#include <omp.h>
#include <mpi.h>

static int processes_cnt = 16;
static int threads_cnt = 8;

static double the_func(double x)
{
	return (x - 3) * (x - 1) * (x + 1) / 5;
}

static double rectangel_method(double start, double dx, int cnt) {
	double sum = 0;
	double curX;
#pragma omp parallel for num_threads (threads_cnt) reduction (+:sum) private (curX)
	for (int i = 0; i < cnt; ++i) {
		curX = start + i * dx;
		sum += the_func(curX);
	}
	return sum * dx;
}

int main(int argc, char* argv[]) {
	double start = -3;
	double end = 4;
	int prec = 100000000;
	double dx = (end - start) / prec;
	
	int rank, size;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	double res = rectangel_method(start, dx, prec + 1);
	printf("%.9lf\n", res);
	
	MPI_Finalize();
	
	return 0;
}
