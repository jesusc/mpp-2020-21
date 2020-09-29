/********************************************************************
* Francisco Almeida, Domingo Giménez, José Miguel Mantas, Antonio M. Vidal:
* Introducción a la programación paralela,
* Paraninfo Cengage Learning, 2008
*
* Capítulo 3, Código 3.16
* Cálculo de PI en OpenMP
*********************************************************************/

#include <stdio.h>
#include <math.h>
#include <omp.h>

double f(double x)
{
  return sqrt(1-pow(x,2.));
}

int main(int argc, char *argv[]) {
    int  n, i;
    double PI25DT = 3.141592653589793238462643;
    double pi, h, sum, x;

    printf("Numero de intervalos a usar: ");
    scanf("%d",&n);
    // Inicializa el número de intervalos a n
    h   = 1.0 / (double) n;
    sum = 0.0;
            // La variable pi es una variable de reducción mediante suma
    #pragma omp parallel for reduction(+:pi) private(x, i)
    for (i = 1; i <= n; i ++) {
        x = h * ((double)i - 0.5);
        pi += f(x);
    }
    pi = 4. * h * pi;
    printf("\npi es aproximadamente %.16f, el error es %.16f\n", pi, fabs(pi - PI25DT)); 
}

