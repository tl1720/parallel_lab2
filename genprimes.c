#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <string.h>

int main (int argc, char *argv[]) {
        if (argc < 3 || argc > 3) {
                printf("error argument number\n");
                return 0;
        }

        long prime_size  = atoi(argv[1]) + 1;
        int thread_count = atoi(argv[2]);
        long check_bound = (prime_size+1)/2;
        //long check_bound = sqrt(prime_size);
        char *prime      = (char *)malloc(prime_size+1);
        memset(prime, 1, prime_size);
        
        omp_set_num_threads(thread_count);
        double tstart = 0.0, ttaken = 0.0;
        tstart = omp_get_wtime();

	long base = 2;
        while (base <= check_bound) {
        	if (prime[base] && base*base <= prime_size) {
                	# pragma omp parallel for
                        for (long j = base*base;j <= prime_size;j += base) {
                       		prime[j] = 0;
                        }
                }
                base++;
        }

        ttaken = omp_get_wtime() - tstart;
        printf("Time take for the main part: %f\n",ttaken);

        char file_name[255] = {0};
        sprintf(file_name, "%s.txt", argv[1]);
        FILE *fp = fopen(file_name, "w");
        if (!fp) {
                printf("can't open a file\n");
                return 0;
        }

	int count = 0;
        long previous_prime = 2;
        for (long i = 2;i <= prime_size;i++) {
                if (prime[i]) {
                	count++;
                        //printf("%ld ", i);
                        fprintf(fp, "%d, %ld, %ld\n",count,i,(i-previous_prime));
                      	previous_prime = i;
                 }
	}
                    
       // release resource
       fclose(fp);
       free(prime); 

       return 0;         
}
