#include <R.h>
#include <iostream>
#include <assert.h>
using namespace std;


// simulate row-major, 1-indexed lookup, as per R conventions
// though really it's column-major and 0-indexed underneath

int mi(int i, int j, int nrow) {
  assert(i <= nrow && i >= 1);
  assert(j >= 1);
  return  nrow * (j-1)  +  (i-1);
}

int mi3(int i, int j, int k,  int ni, int nj) {
  assert(i >= 1 && j >= 1 && k >= 1);
  assert(i <= ni && j <= nj);
  return  (k-1) * ni*nj  +  (j-1) * ni  +  (i-1);
}

extern "C" {

  void matrix_test(int* matrix, int* nrow, int* ncol) 
  {
    cout << "hi there " << *nrow << " " << *ncol <<  endl;
    for (int i=1; i <= *nrow; i++) {
      cout << "ROW " << i << endl;
      for (int j=1; j <= *ncol; j++) {
        // cout << "COL " << j << " -> " << *(matrix + j*(*nrow) + i) << endl;
        cout << "COL " << j << " -> " << matrix[mi(i,j,*nrow)] << endl;
      }
    }
  }


  void calculate_anno_llrs_c(double *wm, int *w, int *r, int *N, int *K, int *W, double * alm) 
  {
    for (int i=1; i <= *N; i++) {
      // cout << "anno " << i << "  worker: " << w[i-1] << "  response: " << r[i-1] << endl;
      for (int real_label=1; real_label <= *K; real_label++) {
        double llr = wm[mi3( w[i-1], r[i-1], real_label,  *W,*K )];
        alm[mi( i, real_label,  *N )] = llr;
      }
    }
  }


  // .C("calculate_anno_llrs_c", as.double(aw), as.integer(response),
  //   as.integer(N), as.integer(length(cs)),
  //   output=as.double(anno_llr_matrix))
    
  // void calculate_anno_llrs_c(double *aw, int *r, int *N, int *K, double * alm) 
  // {
  //   for (int i=0; i < *N; i++) {
  //     cout << "row " << i << "  response: " << r[i] << endl;
  //     for (int real_label=1; real_label <= *K; real_label++) {
  //       double llr = aw[mi3( i,r[i]-1, real_label-1,  *N,*K )];
  //       alm[ mi(i,real_label-1,  *N) ] = llr;
  //     }
  //   }
  // }

}
