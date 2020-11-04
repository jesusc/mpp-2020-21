#include <stdio.h>
#include <stdlib.h>
#include <math.h>


double f(double x)
{
  return sqrt(1-pow(x,2.));
}

int main(int argc, char *argv[]) {
    long  n, i;
    double PI25DT = 3.141592653589793238462643;
    double pi, h, sum, x;

    if (argc < 2) {
      printf("Argumentos inválidos. Usar pi <intervalos>\n");
      exit(-1);
    }
    
    n = atol(argv[1]);
    
    // Inicializa el número de intervalos a n
    h   = 1.0 / (double) n;
    sum = 0.0;
    
    for (i = 1; i <= n; i ++) {
        x = h * ((double)i - 0.5);
        pi += f(x);
    }

    pi = 4. * h * pi;
    printf("\npi es aproximadamente %.16f, el error es %.16f\n", pi, fabs(pi - PI25DT)); 
}

