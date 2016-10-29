/* NB: this is a subset of the routines in the CSPARSE package by
 Tim Davis et. al., for the full package please visit
 http://www.cise.ufl.edu/research/sparse/CSparse/ */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "cs.h"
#include "constants.h"


/* Create Compressed-Column-Sparse matrix from existing arrays (no MALLOC) */
csc* csc_matrix(c_int m, c_int n, c_int nnz, c_float* x, c_int* i, c_int* p)
{
	csc* M = (csc *)c_malloc(sizeof(csc));
	M->m = m;
	M->n = n;
	M->nnz = nnz;
	M->x = x;
    M->i = i;
    M->p = p;
	// if (M->p) M->p[n] = nnz;  // useless
	return M;
}


/* Create uninitialized Compressed-Column-Sparse matrix (uses MALLOC) */
csc* new_csc_matrix(c_int m, c_int n, c_int nnz)
{
    c_float * x = (c_float *)malloc((nnz)*sizeof(c_float));
    c_int * i = (c_int *)malloc((nnz)*sizeof(c_int));
    c_int * p = (c_int *)malloc((n+1)*sizeof(c_int));
    p[n] = nnz;  // Last element corresponds to number of nonzeros
	return csc_matrix(m, n, nnz, x, i, p);
}

/* Free sparse matrix (uses FREE) */
void free_csc_matrix(csc * M)
{
    // Free allocated memory
    if (M->x) c_free(M->x);
    if (M->i) c_free(M->i);
    if (M->p) c_free(M->p);

    // Free actual structure
    c_free(M);

}


/* Convert sparse to dense */
c_float * csc_to_dns(csc * M)
{
    c_int i, j=0;  // Predefine row index and column index

    // Initialize matrix of zeros
    c_float * A = (c_float *)c_calloc(M->m * M->n, sizeof(c_float));

    // Allocate elements
    for (c_int idx = 0; idx < M->nnz; idx++)
    {
        // Get row index
        i = M->i[idx];

        // Get column index (increase if necessary)
        if (M->p[idx] == idx & j < M->n) j++;

        // Assign values to A
        A[i*M->m+j] = M->x[idx];
    }
    return A;
}




/* ================================= DEBUG FUNCTIONS ======================= */
#if PRINTLEVEL > 2

/* Print a sparse matrix */
void print_csc_matrix(csc* M, char * name)
{
    c_print("%s :\n", name);
    c_int j, i, row_strt, row_stop;
    c_int k = 0;
    for(j=0; j<M->n; j++){
        row_strt = M->p[j];
        row_stop = M->p[j+1];
        if (row_strt == row_stop)
            continue;
        else {
            for(i=row_strt; i<row_stop; i++ ){
                c_print("\t(%3u,%3u) = %g\n", (int)M->i[i]+1, (int)j+1, M->x[k++]);
            }
        }
    }
}


/* Print a dense matrix */
void print_dns_matrix(c_float * M, c_int m, c_int n, char *name)
{
    c_print("%s = [\t", name);
    for(c_int i =0; i<m; i++){
        for(c_int j=0; j<n; j++){
            if (j < n - 1)
                c_print("% 14.12e,  ",M[i*n+j]);
            else
                c_print("% 14.12e ",M[i*n+j]);
        }
        if (i < m - 1){
            c_print("\n\t");
        }
        else
        {
            c_print("]");
        }
    }
    c_print("\n");
}



#endif
