#ifndef MM_H
#define MM_H
#include <stdio.h>
typedef struct{
    // matrix multiplication task
    // Z = X@Y  (@: matrix multiplication)
    // shape of X: m*k; shape of Y: k*n; shape of Z: m*n
    int task_id, m, k, n, *X, *Y, *Z;
    /** DO NOT MODIFY ABOVE **/
    // you can add your variables:
    
}MMData;

void mm_compute(MMData* data);

void load_MMData(FILE* fp, MMData* data);
void print_MMData(MMData* data);
#endif /* MM_H */