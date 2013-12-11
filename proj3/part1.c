#include <emmintrin.h>
#include <string.h>
#include <stdio.h> 
#define KERNX 3 //this is the x-size of the kernel. It will always be odd.
#define KERNY 3 //this is the y-size of the kernel. It will always be odd.
int conv2D(float* in, float* out, int data_size_X, int data_size_Y,
           float* kernel)
{
  #pragma omp parallel
  {
  // the x coordinate of the kernel's center
  int kern_cent_X = (KERNX - 1)/2;
  // the y coordinate of the kernel's center
  int kern_cent_Y = (KERNY - 1)/2;
 
  int new_x = data_size_X+ 2*kern_cent_X;
  int new_y = data_size_Y+ 2*kern_cent_Y;
  float arr[new_x * new_y];
  int arr_size = new_x * new_y;
  float output;
  int item, current_operands, pos_x, pos_y, out_index, k_ind;

 
  int kern_size = KERNX * KERNY;    //new switch
  float flip_kernel[kern_size];
  for (int t = 0; t < kern_size; t++) {
    flip_kernel[t] = kernel[kern_size-1-t];
  }
  memset(arr, 0.0, sizeof(float)*arr_size); //pads the matrix with zeroes
  for (int i = 0; i < data_size_Y; i++) {
    memcpy(arr + ((i+kern_cent_Y) * new_x) + kern_cent_X, in + (i * data_size_X), sizeof(float) * data_size_X);
  }

  __m128 kern_vec, in_vec, product, add_vec; //intrinsics
  //#pragma omp parallel for 
  for(int y = 0; y < data_size_Y; y++){ // the y coordinate of theoutput location we're focusing on
    for(int x = 0; x < data_size_X; x++){ // the x coordinate of the output location we're focusing on
      item = x + (y * data_size_X);
      add_vec = _mm_setzero_ps();
      k_ind = 0;
      for(int j = -kern_cent_Y; j <= kern_cent_Y; j++){ // kernel unflipped y coordinate
        current_operands = x + kern_cent_X + ((y + kern_cent_Y) + j)*new_x;
        for(int i = -kern_cent_X; i <= kern_cent_X; i++){ // kernel unflipped x coordinate
          output += flip_kernel[k_ind] * arr[i + current_operands];
          /* failed
           kern_vec = _mm_load1_ps(flip_kernel+(kern_cent_X-i + ((kern_cent_Y-j)* KERNX)));
           in_vec = _mm_loadu_ps(arr + ((x + i + 1) + (j + y + 1) * new_X));
           product = _mm_mul_ps(kern_vec, in_vec);
           add_vec = _mm_add_ps(product, add_vec);
           
           */
          k_ind++;
        }
      }
      out[item] = output;
      output = 0;
      //_mm_storeu_ps(out + item, add_vec);
    }

   
  }
 }
 
  return 1;
}
 
 