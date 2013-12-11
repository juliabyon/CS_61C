#include <stdio.h> 
#include <emmintrin.h>
#include <string.h>
#include <omp.h>
#define KERNX 3 //this is the x-size of the kernel. It will always be odd.
#define KERNY 3 //this is the y-size of the kernel. It will always be odd.
int conv2D(float* in, float* out, int data_size_X, int data_size_Y,
                    float* kernel)
{ 
 
    int kern_cent_X = (KERNX - 1)/2; 
    int kern_cent_Y = (KERNY - 1)/2;
    int padded_size_X = data_size_X + 2;
    int padded_size_Y = data_size_Y + 2;
    int data_size_X_rounded = (data_size_X/4)*4;

    //Pad input matrix with zeros all around.
    //increases speed when padded manually (calloc vs malloc)
    float padded_matrix[padded_size_X * padded_size_Y];
    #pragma omp parallel for 
    for (int q = 0; q < (padded_size_X*padded_size_Y); q++) {
        padded_matrix[q] = 0;
    }
    #pragma omp parallel for firstprivate(padded_size_X, data_size_X)
    for (int i = 0; i < data_size_Y; i++) {
        memcpy(padded_matrix + (i+1) * padded_size_X + 1, in + i * data_size_X, sizeof(float) * data_size_X);
    }
  
    // main convolution loop
    //parallelize entire for loop
  #pragma omp parallel for firstprivate(data_size_Y, data_size_X_rounded,kern_cent_Y, kern_cent_X, data_size_X)
    for(int y = 0; y < data_size_Y; y++) { // the y coordinate of the output location we're focusing on
            for(int x = 0; x < data_size_X_rounded; x+=4) { // the x coordinate of the output location we're focusing on
                __m128 out_vect = _mm_setzero_ps();
                __m128 kernel_vect, padded_vect, product;
                //privatizes needed variables
                #pragma omp firstprivate(j, kern_pos_Y, padded_pos_Y, i, out_vect, kernel_vect, padded_vect, product)
                    int j = -kern_cent_Y;
                    int kern_pos_Y = kern_cent_Y-j;
                    int padded_pos_Y = y+j+1;
                    int i = -kern_cent_X;
                    padded_pos_Y = y;
                    //unrolling inner two for loops
                    kern_pos_Y = 2;
                    kernel_vect = _mm_load1_ps(kernel + (kern_cent_X-i) + ((kern_pos_Y) * KERNX));
                    padded_vect = _mm_loadu_ps(padded_matrix + (x+i+1) + (padded_pos_Y) * padded_size_X);
                    product = _mm_mul_ps(kernel_vect, padded_vect);
                    out_vect = _mm_add_ps(out_vect, product);

                    kernel_vect = _mm_load1_ps(kernel + (kern_cent_X) + ((kern_pos_Y) * KERNX));
                    padded_vect = _mm_loadu_ps(padded_matrix + (x+i+2) + (padded_pos_Y) * padded_size_X);
                    product = _mm_mul_ps(kernel_vect, padded_vect);
                    out_vect = _mm_add_ps(out_vect, product);

                    kernel_vect = _mm_load1_ps(kernel + (kern_cent_X-1) + ((kern_pos_Y) * KERNX));
                    padded_vect = _mm_loadu_ps(padded_matrix + (x+i+3) + (padded_pos_Y) * padded_size_X);
                    product = _mm_mul_ps(kernel_vect, padded_vect);
                    out_vect = _mm_add_ps(out_vect, product);


                    //j = 0
                    padded_pos_Y = y+1;
                    kern_pos_Y = 1;
                    kernel_vect = _mm_load1_ps(kernel + (kern_cent_X-i) + ((kern_pos_Y) * KERNX));
                    padded_vect = _mm_loadu_ps(padded_matrix + (x+i+1) + (padded_pos_Y) * padded_size_X);
                    product = _mm_mul_ps(kernel_vect, padded_vect);
                    out_vect = _mm_add_ps(out_vect, product);

                    kernel_vect = _mm_load1_ps(kernel + (kern_cent_X) + ((kern_pos_Y) * KERNX));
                    padded_vect = _mm_loadu_ps(padded_matrix + (x+i+2) + (padded_pos_Y) * padded_size_X);
                    product = _mm_mul_ps(kernel_vect, padded_vect);
                    out_vect = _mm_add_ps(out_vect, product);

                    kernel_vect = _mm_load1_ps(kernel + (kern_cent_X-1) + ((kern_pos_Y) * KERNX));
                    padded_vect = _mm_loadu_ps(padded_matrix + (x+i+3) + (padded_pos_Y) * padded_size_X);
                    product = _mm_mul_ps(kernel_vect, padded_vect);
                    out_vect = _mm_add_ps(out_vect, product);


                    //j = 1
                    padded_pos_Y = y+2;
                    kern_pos_Y = 0;
                    kernel_vect = _mm_load1_ps(kernel + (kern_cent_X-i) + ((kern_pos_Y) * KERNX));
                    padded_vect = _mm_loadu_ps(padded_matrix + (x+i+1) + (padded_pos_Y) * padded_size_X);
                    product = _mm_mul_ps(kernel_vect, padded_vect);
                    out_vect = _mm_add_ps(out_vect, product);

                    kernel_vect = _mm_load1_ps(kernel + (kern_cent_X) + ((kern_pos_Y) * KERNX));
                    padded_vect = _mm_loadu_ps(padded_matrix + (x+i+2) + (padded_pos_Y) * padded_size_X);
                    product = _mm_mul_ps(kernel_vect, padded_vect);
                    out_vect = _mm_add_ps(out_vect, product);

                    kernel_vect = _mm_load1_ps(kernel + (kern_cent_X-1) + ((kern_pos_Y) * KERNX));
                    padded_vect = _mm_loadu_ps(padded_matrix + (x+i+3) + (padded_pos_Y) * padded_size_X);
                    product = _mm_mul_ps(kernel_vect, padded_vect);
                    out_vect = _mm_add_ps(out_vect, product);

                    _mm_storeu_ps(out + (x+y*data_size_X), out_vect); 
            }
                //MAYBE SEPERATE THIS. TAKE OUTSIDE Y LOOP, SO CAN REMOVE FROM PARALLELIZATION
                //tail loop for catching mod 4s  
                for (int x = data_size_X_rounded; x < data_size_X; x++){
                    for(int j = -kern_cent_Y; j <= kern_cent_Y; j++) { //kernel unflipped y coordinate
                        for(int i = -kern_cent_X; i <= kern_cent_X; i++) { //kernel unflipped x coordinate
                            //#pragma omp critical
                            out[x + y*data_size_X] += kernel[(kern_cent_X-i) + (kern_cent_Y-j)*KERNX] * padded_matrix[(x+i+1) + (y+j+1)*padded_size_X]; //RECREATE SO CAN EFFICIENTLY CRITICAL
                        }
                    }
                }
            }
    return 1;
}



                            