#include <stdio.h>
#include <stdlib.h>
#include<omp.h>

double step, pi;

int main(int argc, char *argv[]){
	int i;
	double x, sum = 0.0, start_time, end_time;
	printf("%s\n", argv[1]);

	long num_steps = atol(argv[1]);
	step = 1.0 / (double)num_steps;
	
	#pragma omp parallel private(x) shared(sum)
	{
		start_time = omp_get_wtime();
		#pragma omp for schedule(static, 10)
		for (i = 0; i< num_steps; i++){
			x = (i + 0.5) * step;
			sum = sum + 4.0 / (1.0 + x*x);
		}
		pi = step * sum;	
	}	
	end_time = omp_get_wtime() - start_time;
	
	printf("Pi = %f\n", pi);
	printf("Parallel Region Execution Time : %lf\n", end_time);
	return 0;
}
