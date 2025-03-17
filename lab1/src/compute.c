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

void zero_z()
{
	for (int i = 0; i != m; ++i)
	{
		for (int j = 0; j != n; ++j)
		{
			Z[i][j] = 0;
		}
	}
}

void compute_row_major_mnk()
{
	zero_z();
	for (int i = 0; i != m; ++i)
	{
		for (int j = 0; j != n; ++j)
		{
			for (int l = 0; l != k; ++l)
			{
				Z[i][j] += X[i][l] * Y[l][j];
			}
		}
	}
}

void compute_row_major_mkn()
{
	// TODO: task 1
	zero_z();
	for (int i = 0; i ^ m; ++i)
	{
		for (int l = 0; l ^ k; ++l)
		{
			for (int j = 0; j ^ n; ++j)
			{
				Z[i][j] += X[i][l] * Y[l][j];
			}
		}
	}
}

void compute_row_major_kmn()
{
	// TODO: task 1
	zero_z();
	for (int l = 0; l != k; ++l)
	{
		for (int i = 0; i != m; ++i)
		{
			for (int j = 0; j != n; ++j)
			{
				Z[i][j] += X[i][l] * Y[l][j];
			}
		}
	}
}

void compute_row_major_nmk()
{
	// TODO: task 1
	zero_z();
	for (int j = 0; j != n; ++j)
	{
		for (int i = 0; i != m; ++i)
		{
			for (int l = 0; l != k; ++l)
			{
				Z[i][j] += X[i][l] * Y[l][j];
			}
		}
	}
}

void compute_row_major_nkm()
{
	// TODO: task 1
	zero_z();
	for (int j = 0; j != n; ++j)
	{
		for (int l = 0; l != k; ++l)
		{
			for (int i = 0; i != m; ++i)
			{
				Z[i][j] += X[i][l] * Y[l][j];
			}
		}
	}
}

void compute_row_major_knm()
{
	// TODO: task 1
	zero_z();
	for (int l = 0; l != k; ++l)
	{
		for (int j = 0; j != n; ++j)
		{
			for (int i = 0; i != m; ++i)
			{
				Z[i][j] += X[i][l] * Y[l][j];
			}
		}
	}
}

void compute_y_transpose_mnk()
{
	// TODO: task 2
	zero_z();
	for (int i = 0; i != m; ++i)
	{
		for (int j = 0; j != n; ++j)
		{
			for (int l = 0; l != k; ++l)
			{
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
					for (int ll = 0; ll < B; ll++)
					{
						for (int ii = 0; ii < B; ii++)
						{
							for (int jj = 0; jj < B; jj++)
							{
								Z[i + ii][j + jj] += X[i + ii][l + ll] * Y[l + ll][j + jj];
							}
						}
					}
				}
			}
		}
	}
	else //
	{
		for (int i = 0; i < m; i += B)
		{
			for (int j = 0; j < n; j += B)
			{
				for (int l = 0; l < k; l += B)
				{
					for (int ll = 0; ll < B && l + ll < k; ll++)
					{
						for (int ii = 0; ii < B && i + ii < m; ii++)
						{
							for (int jj = 0; jj < B && j + jj < n; jj++)
							{
								Z[i + ii][j + jj] += X[i + ii][l + ll] * Y[l + ll][j + jj];
							}
						}
					}
				}
			}
		}
	}
}

void compute_row_major_shit()
{
	zero_z();
	register uint64_t reg = 0;
	register int i, j, l;
	for (i = 0; i != m; ++i)
	{
		for (j = 0; j != n; ++j)
		{
			for (l = 0; l != k; ++l)
			{
				reg += X[i][l] * YP[j][l];

			}
			Z[i][j] = reg;
			reg = 0;
		}
	}
	/*zero_z();
	const int B = 8;
	if (m % B == 0 && n % B == 0 && k % B == 0)
	{
		for (int i = 0; i < m; i += B)
		{
			for (int j = 0; j < n; j += B)
			{
				for (int ii = 0; ii < B; ii++)
				{
					for (int jj = 0; jj < B; jj++)
					{
						for (int ll = 0; ll < k; ll++)
						{
							Z[i + ii][j + jj] += X[i + ii][ll] * YP[j + jj][ll];
						}
					}
				}
			}
		}
	}
	else //
	{
		for (int i = 0; i < m; i += B)
		{
			for (int j = 0; j < n; j += B)
			{
				for (int ii = 0; ii < B && i + ii < m; ii++)
				{
					for (int jj = 0; jj < B && j + jj < n; jj++)
					{
						for (int ll = 0; ll < k; ll++)
						{
							Z[i + ii][j + jj] += X[i + ii][ll] * YP[j + jj][ll];
						}
					}
				}
			}
		}
	}*/
}

void compute_row_major_mnk_lu2()
{
	// TODO: task 2
	zero_z();
	for (int i = 0; i != m; ++i)
	{
		for (int j = 0; j != n; ++j)
		{
			for (int l = 0; l != k; l += 2)
			{
				Z[i][j] += X[i][l] * Y[l][j];
				Z[i][j] += X[i][l + 1] * Y[l + 1][j];
			}
		}
	}
}


void compute_simd()
{
#ifdef SIMD
	register int i, j, l;
	zero_z();
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			uint64x2_t sum = vdupq_n_u64(0);
			
			// Process 4 elements at a time with unrolled loop
			for (l = 0; l < k; l += 16)
			{
				// Process 16 elements in groups of 4
				uint32x4_t x1 = vld1q_u32(X32[i] + l);
				uint32x4_t y1 = vld1q_u32(YP32[j] + l);
				uint64x2_t p1 = vpaddlq_u32(vmulq_u32(x1, y1));
				
				uint32x4_t x2 = vld1q_u32(X32[i] + l + 4);
				uint32x4_t y2 = vld1q_u32(YP32[j] + l + 4);
				uint64x2_t p2 = vpaddlq_u32(vmulq_u32(x2, y2));
				
				uint32x4_t x3 = vld1q_u32(X32[i] + l + 8);
				uint32x4_t y3 = vld1q_u32(YP32[j] + l + 8);
				uint64x2_t p3 = vpaddlq_u32(vmulq_u32(x3, y3));
				
				uint32x4_t x4 = vld1q_u32(X32[i] + l + 12);
				uint32x4_t y4 = vld1q_u32(YP32[j] + l + 12);
				uint64x2_t p4 = vpaddlq_u32(vmulq_u32(x4, y4));
				
				sum = vaddq_u64(sum, vaddq_u64(vaddq_u64(p1, p2), vaddq_u64(p3, p4)));
			}
			
			// Sum the final result
			Z[i][j] = vgetq_lane_u64(sum, 0) + vgetq_lane_u64(sum, 1);
		}
	}
#endif
}

/*for (i = 0; i < m; ++i)
{
	for (j = 0; j < n; ++j)
	{
		uint64x2_t res = vdupq_n_u64(0);
		for (l = 0; l < k; l += 4)
		{
			uint32x4_t x = vld1q_u32(X32[i] + l);
			uint32x4_t y = vld1q_u32(YP32[j] + l);
			uint32x4_t z = vmulq_u32(x, y);
			uint64x2_t s = vpaddlq_u32(z);
			res = vaddq_u64(res, s);
		}
		Z[i][j] = vgetq_lane_u64(res, 0) + vgetq_lane_u64(res, 1);
	}
}*/

uint64_t elapsed(const struct timespec start, const struct timespec end)
{
	struct timespec result;
	result.tv_sec = end.tv_sec - start.tv_sec;
	result.tv_nsec = end.tv_nsec - start.tv_nsec;
	if (result.tv_nsec < 0)
	{
		--result.tv_sec;
		result.tv_nsec += SEC;
	}
	uint64_t res = result.tv_sec * SEC + result.tv_nsec;
	return res;
}

uint64_t compute()
{
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);

	switch (COMPUTE_SELECT)
	{
	case COMPUTE_ROW_MAJOR_MNK:
		// printf("COMPUTE_ROW_MAJOR_MNK\n");
		compute_row_major_mnk();
		break;
	case COMPUTE_ROW_MAJOR_MKN:
		// printf("COMPUTE_ROW_MAJOR_MKN\n");
		compute_row_major_mkn();
		break;
	case COMPUTE_ROW_MAJOR_KMN:
		// printf("COMPUTE_ROW_MAJOR_KMN\n");
		compute_row_major_kmn();
		break;
	case COMPUTE_ROW_MAJOR_NMK:
		// printf("COMPUTE_ROW_MAJOR_NMK\n");
		compute_row_major_nmk();
		break;
	case COMPUTE_ROW_MAJOR_NKM:
		// printf("COMPUTE_ROW_MAJOR_NKM\n");
		compute_row_major_nkm();
		break;
	case COMPUTE_ROW_MAJOR_KNM:
		// printf("COMPUTE_ROW_MAJOR_KNM\n");
		compute_row_major_knm();
		break;
	case COMPUTE_Y_TRANSPOSE_MNK:
		// printf("COMPUTE_Y_TRANSPOSE_MNK\n");
		compute_y_transpose_mnk();
		break;
	case COMPUTE_ROW_MAJOR_MNKKMN_B32:
		// printf("COMPUTE_ROW_MAJOR_MNKKMN_B32\n");
		compute_row_major_mnkkmn_b32();
		break;
	case COMPUTE_ROW_MAJOR_MNK_LU2:
		// printf("COMPUTE_ROW_MAJOR_MNK_LU2\n");
		compute_row_major_mnk_lu2();
		break;
	case COMPUTE_ROW_MAJOR_SHIT:
		// printf("COMPUTE_ROW_MAJOR_SHIT\n");
		compute_row_major_shit();
		break;
	case COMPUTE_SIMD:
		// printf("COMPUTE_SIMD\n");
		compute_simd();
		break;
	default:
		printf("Unreachable!");
		return 0;
	}

	clock_gettime(CLOCK_MONOTONIC, &end);
	return elapsed(start, end);
}
