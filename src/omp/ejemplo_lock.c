#include <stdio.h>
#include <omp.h>

// OpenMP API pag 272

void skip(int i) {}
void work(int i) {}

int main()
{
  omp_lock_t	lck;
  int		id;
  
  omp_init_lock(&lck);
  
  #pragma omp parallel shared(lck) private(id)
  {
    id=omp_get_thread_num();
    
    omp_set_lock(&lck);
    // solamente un thread en cada momento puede ejecutar esto
    printf("My thread id is %d.\n",id);
    omp_unset_lock(&lck);
    
    while (! omp_test_lock(&lck))
    {
      skip(id);  //mientras no tenga el lock me dedico a hacer otra cosa
    }//while
    
    work(id);	//ahora tengo el lock, entonces hago el trabajo
    
    omp_unset_lock(&lck);
  
  }//parallel
  
  omp_destroy_lock(&lck);
  
}