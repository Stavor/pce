#include <stdio.h>
#include <omp.h>

static int threads_cnt = 16;
static __int64 A[10000005];
static int _10M = 10000000;

__int64 static_array_sum() {
	__int64 sum = 0;
#pragma omp parallel for num_threads (threads_cnt) reduction(+:sum)
	for (intptr_t i = 0; i < _10M; i++)
		sum += A[i]; 
	return sum;
}

int main() {
	for (intptr_t i = 0; i < _10M; i++)
		A[i] = i;
	__int64 res = static_array_sum();
	printf("%I64d\n", res); //correct res is 49999995000000
	return 0;
}