#include <stdio.h>
#include <windows.h>
#include <omp.h>

static int threads_cnt = 7;

int main() {
	printf("Process num: %d\n\n", omp_get_num_procs());
	int threadId;
#pragma omp parallel num_threads (threads_cnt) firstprivate (threadId)
	{
		threadId = omp_get_thread_num();
		if(threadId % 2 == 0)
			Sleep(2000);
		printf("%d: Hello World!\n", threadId);
	}
	printf("The End\n");
	return 0;
}