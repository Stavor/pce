#include <stdio.h>
#include <omp.h>

static int threads_cnt = 16;

static double the_func(double x)
{
	return (x - 3) * (x - 1) * (x + 1) / 5;
}

static double rectangel_method(double start, double dx, int cnt) {
	double sum = 0;
	double curX;
#pragma omp parallel for num_threads (threads_cnt) reduction (+:sum) private (curX)
	for (intptr_t i = 0; i < cnt; ++i) {
		curX = start + i * dx;
		sum += the_func(curX);
	}
	return sum * dx;
}

int main() {
	double start = -3;
	double end = 4;
	int prec = 100000000;
	double dx = (end - start) / prec;

	double st = omp_get_wtime();
	double res = rectangel_method(start, dx, prec + 1);
	double fn = omp_get_wtime();
	
	printf("%.9lf / time: %.6lf\n", res, fn - st);
	return 0;
}