
#include <math.h>

void distance_kernel_c(int n, const float *X1, const float *X2, const float *Y1, const float *Y2, float *Z) {
	
	for (int i = 0; i < n; i++) {
		float dx = X2[i] - X1[i];
		float dy = Y2[i] - Y1[i];
		float dx_squared = dx * dx;
		float dy_squared = dy * dy;

		float sum_of_squares = dx_squared + dy_squared;
		Z[i] = sqrtf(sum_of_squares);
	}
}