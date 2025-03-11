#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#ifdef SIMD
#include <arm_neon.h>
#endif

#include "common.h"
#include "compute.h"

void zero_z() {
    for (int i = 0; i != m; ++i) {
        for (int j = 0; j != n; ++j) {
            Z[i][j] = 0;
        }
    }
}

void compute_row_major_mnk() {
    zero_z();
    for (int i = 0; i != m; ++i) {
        for (int j = 0; j != n; ++j) {
            for (int l = 0; l != k; ++l) {
                Z[i][j] += X[i][l] * Y[l][j];
            }
        }
    }
}

void compute_row_major_mkn() {
    // TODO: task 1
		zero_z();
		for (int i = 0; i != m; ++i) {
			for (int l = 0; l != k; ++l) {
				for (int j = 0; j != n; ++j) {
					Z[i][j] += X[i][l] * Y[l][j];
				}
			}
		}
}

void compute_row_major_kmn() {
    // TODO: task 1
		zero_z();
		for (int l = 0; l != k; ++l) {
			for (int i = 0; i != m; ++i) {
				for (int j = 0; j != n; ++j) {
					Z[i][j] += X[i][l] * Y[l][j];
				}
			}
		}
}

void compute_row_major_nmk() {
    // TODO: task 1
		zero_z();
		for (int j = 0; j != n; ++j) {
			for (int i = 0; i != m; ++i) {
				for (int l = 0; l != k; ++l) {
					Z[i][j] += X[i][l] * Y[l][j];
				}
			}
		}
}

void compute_row_major_nkm() {
    // TODO: task 1
		zero_z();
		for (int j = 0; j != n; ++j) {
			for (int l = 0; l != k; ++l) {
				for (int i = 0; i != m; ++i) {
					Z[i][j] += X[i][l] * Y[l][j];
				}
			}
		}
}

void compute_row_major_knm() {
    // TODO: task 1
		zero_z();
		for (int l = 0; l != k; ++l) {
			for (int j = 0; j != n; ++j) {
				for (int i = 0; i != m; ++i) {
					Z[i][j] += X[i][l] * Y[l][j];
				}
			}
		}
}

void compute_y_transpose_mnk() {
    // TODO: task 2
		zero_z();
		for (int i = 0; i != m; ++i) {
			for (int j = 0; j != n; ++j) {
				for (int l = 0; l != k; ++l) {
					Z[i][j] += X[i][l] * YP[j][l];
				}
			}
		}
}

void compute_row_major_mnkkmn_b32()
{
	zero_z();
	const int B = 32;
	if (m % B == 0 && n % B == 0 && k % B == 0)
	{
		for (int i = 0; i < m; i += B)
		{
			for (int j = 0; j < n; j += B)
			{
				for (int l = 0; l < k; l += B)
				{
					for (int ii = 0; ii < B; ii++)
					{
						for (int jj = 0; jj < B; jj++)
						{
							for (int ll = 0; ll < B; ll++)
							{
								Z[i + ii][j + jj] += X[i + ii][l + ll] * Y[l + ll][j + jj];
							}
						}
					}
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < m; i += B)
		{
			int i_end = (i + B < m) ? i + B : m;
			for (int j = 0; j < n; j += B)
			{
				int j_end = (j + B < n) ? j + B : n;
				for (int l = 0; l < k; l += B)
				{
					int l_end = (l + B < k) ? l + B : k;
					// 统一使用动态边界处理，移除if-else分支
					for (int li = l; li < l_end; ++li)
					{ // K维度块内遍历
						for (int ii = i; ii < i_end; ++ii)
						{																// M维度块内遍历
							float x_val = X[ii][li]; // 行主序访问X[ii][li]
							for (int jj = j; jj < j_end; ++jj)
							{																						// N维度块内遍历
								Z[ii][jj] += x_val * Y[li][jj]; // 行主序访问Y[li][jj]
							}
						}
					}
				}
			}
		}
	}
}

void compute_row_major_mnk_lu2() {
    // TODO: task 2
		zero_z();
		for (int i = 0; i != m; ++i) {
			for (int j = 0; j != n; ++j) {
				for (int l = 0; l != k; l+=2) {
					Z[i][j] += X[i][l] * Y[l][j];
					Z[i][j] += X[i][l+1] * Y[l+1][j];
				}
			}
		}
}

void compute_simd() {
#ifdef SIMD
	zero_z();
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			int32x4_t sum = vdupq_n_s32(0);
			// Handle main chunk
			for (int l = 0; l < (k/4)*4; l += 4) {
				int32x4_t x = vld1q_s32(&X[i][l]);
				int32_t temp[4] = {Y[l][j], Y[l+1][j], Y[l+2][j], Y[l+3][j]};
				int32x4_t y = vld1q_s32(temp);
				sum = vmlaq_s32(sum, x, y);
			}
			// Accumulate result
			Z[i][j] = vaddvq_s32(sum);
			// Handle remainder
			for (int l = (k/4)*4; l < k; l++) {
				Z[i][j] += X[i][l] * Y[l][j];
			}
		}
	}
#else
	printf("SIMD is not enabled!\n");
	return;
#endif
}

uint64_t elapsed(const struct timespec start, const struct timespec end) {
    struct timespec result;
    result.tv_sec = end.tv_sec - start.tv_sec;
    result.tv_nsec = end.tv_nsec - start.tv_nsec;
    if (result.tv_nsec < 0) {
        --result.tv_sec;
        result.tv_nsec += SEC;
    }
    uint64_t res = result.tv_sec * SEC + result.tv_nsec;
    return res;

}

uint64_t compute() {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    switch (COMPUTE_SELECT) {
        case COMPUTE_ROW_MAJOR_MNK:
            //printf("COMPUTE_ROW_MAJOR_MNK\n");
            compute_row_major_mnk();
            break;
        case COMPUTE_ROW_MAJOR_MKN:
            //printf("COMPUTE_ROW_MAJOR_MKN\n");
            compute_row_major_mkn();
            break;
        case COMPUTE_ROW_MAJOR_KMN:
            //printf("COMPUTE_ROW_MAJOR_KMN\n");
            compute_row_major_kmn();
            break;
        case COMPUTE_ROW_MAJOR_NMK:
            //printf("COMPUTE_ROW_MAJOR_NMK\n");
            compute_row_major_nmk();
            break;
        case COMPUTE_ROW_MAJOR_NKM:
            //printf("COMPUTE_ROW_MAJOR_NKM\n");
            compute_row_major_nkm();
            break;
        case COMPUTE_ROW_MAJOR_KNM:
            //printf("COMPUTE_ROW_MAJOR_KNM\n");
            compute_row_major_knm();
            break;
        case COMPUTE_Y_TRANSPOSE_MNK:
            //printf("COMPUTE_Y_TRANSPOSE_MNK\n");
            compute_y_transpose_mnk();
            break;
        case COMPUTE_ROW_MAJOR_MNKKMN_B32:
            //printf("COMPUTE_ROW_MAJOR_MNKKMN_B32\n");
            compute_row_major_mnkkmn_b32();
            break;
        case COMPUTE_ROW_MAJOR_MNK_LU2:
            //printf("COMPUTE_ROW_MAJOR_MNK_LU2\n");
            compute_row_major_mnk_lu2();
            break;
        case COMPUTE_SIMD:
            //printf("COMPUTE_SIMD\n");
            compute_simd();
            break;
        default:
            printf("Unreachable!");
            return 0;
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    return elapsed(start, end);
}
