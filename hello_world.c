#include<stdio.h>
#include<omp.h>

int fib(int n){
    if(n<2)
        return n;
    return fib(n-1) + fib(n-2);
}

int main(){
    
    int i;
    int N=8;
    #pragma omp parallel
    {
    #pragma omp for
    for(i=0;i<N; i++){
        printf("Thread %d : Fib Sum of %d is %d\n",omp_get_thread_num(), ((i*23/4)+3), fib((i*23/4)+3));
    }
    }
}
