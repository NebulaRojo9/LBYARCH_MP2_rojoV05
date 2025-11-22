
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h> // to define QueryPerformanceCounter


extern void distance_kernel_c(int n, const float *X1, const float *X2, const float *Y1, const float *Y2, const float *Z);
extern void distance_kernel_asm(int n, const float *X1, const float *X2, const float *Y1, const float *Y2, const float *Z);

// --- Global Variable --- 
LARGE_INTEGER frequency;
double microseconds_per_tick;


// FUNCTIONS
void initialize_vectors(int n, float *X1, float *X2, float *Y1, float *Y2) {
	
	srand((unsigned int)time(NULL));
	for (int i = 0; i < n; i++) {
		X1[i] = (float)rand() / (float)(RAND_MAX / 10.0);
		X2[i] = (float)rand() / (float)(RAND_MAX / 10.0);
		Y1[i] = (float)rand() / (float)(RAND_MAX / 10.0);
		Y2[i] = (float)rand() / (float)(RAND_MAX / 10.0);
	}
}

void print_first_ten_elements(const char* version_name, int n, const float* Z) {
	printf("\n--- Results for %s Kernel (First 10 Elements) ---\n", version_name);
	int count = (n < 10) ? n : 10;
	for (int i = 0; i < count; i++) {
		printf("Z[%d] = %.9f\n", i, Z[i]);
	}
}

void check_correctness(int n, const float* Z_c, const float* Z_asm) {
	float max_error = 0.0f;
	int error_count = 0;
	const float TOLERANCE = 1e-6;

	for (int i = 0; i < n; i++) {
		float error = fabsf(Z_c[i] - Z_asm[i]);
		if (error > max_error) {
			max_error = error;
		}
		if (error > TOLERANCE) {
			error_count++;
			if (error_count < 5) {
				printf("Error at index %d: C=%.9f, ASM=%.9f, Error=%.9f\n", i, Z_c[i], Z_asm[i], error);
			}
		}
	}

	printf("\nCorrectness check \n");
	if (error_count == 0) {
		printf("  [o] x86-64 kernel output is CORRECT compared to C kernel (Max Error: %.2e)\n", max_error);
	} else {
		printf(" [x] x86-64 kernel output FAILED correctness check! (%d errors found > %.2e)\n", error_count, TOLERANCE);
		printf("   Max Absolute Error: %.9f\n", max_error);
	}
}

// Windows-specific high-resolution time function
LONGLONG get_time_ticks() {
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	return t.QuadPart;
}

int main() {
	// setup high-quality timer most suitable for Visual Studio
	if (!QueryPerformanceFrequency(&frequency)) {
		fprintf(stderr, "Error: QueryPerformance Frequency failed!\n");
		return 1;
	}
	microseconds_per_tick = 1000000.0 / (double)frequency.QuadPart; 
	//printf("microseconds per tick = %lf", microseconds_per_tick); //results to = 0.100000

	// left bitwise shift operator <<
	const long int N_SIZES[] = { 1 << 20, 1 << 24, 1 << 28 }; 
	const int NUM_SIZES = sizeof(N_SIZES) / sizeof(N_SIZES[0]);
	const int RUNS_PER_VERSION = 50;
	float* X1 = NULL, * X2 = NULL, * Y1 = NULL, * Y2 = NULL, * Z_c = NULL, * Z_asm = NULL;


	for (int sizeIndex = 0; sizeIndex < NUM_SIZES; sizeIndex++) {
		long int n = N_SIZES[sizeIndex];
		long long vector_bytes = (long long)n * sizeof(float);

		printf("\n======================================================\n");
		printf("Processing Vector Size: N = 2^%ld (%ld elements)\n", (int)log2(n), n);
		printf("Total Memory Required: %.2f MB (for 6 vectors)\n", (float)vector_bytes * 6 / (1024 * 1024));
		printf("======================================================\n"); 

		X1 = (float*)malloc(vector_bytes);
		X2 = (float*)malloc(vector_bytes);
		Y1 = (float*)malloc(vector_bytes);
		Y2 = (float*)malloc(vector_bytes);
		Z_c = (float*)malloc(vector_bytes);
		Z_asm = (float*)malloc(vector_bytes);

		if (!X1 || !X2 || !Y1 || !Y2 || !Z_c || !Z_asm) {
			perror("Failed to allocate memory");
			goto cleanup;
		}

		initialize_vectors(n, X1, X2, Y1, Y2);
		printf("Vectors are initialized with random values.\n");

		double total_time_c = 0.0;
		double total_time_asm = 0.0;
		LONGLONG start_ticks, end_ticks;

		//=== C Kernel Execution and Timing ===
		for (int run = 0; run < RUNS_PER_VERSION; run++) {
			start_ticks = get_time_ticks();
			// FROM Instruction: time the kernel portion only
			distance_kernel_c(n, X1, X2, Y1, Y2, Z_c);
			end_ticks = get_time_ticks();
			total_time_c += (double)(end_ticks - start_ticks);
		}
		// convert to milliseconds
		double avg_time_c = total_time_c * microseconds_per_tick / RUNS_PER_VERSION / 1000.0;
		printf("\n [o] C Kernel Complete.\n");
		print_first_ten_elements("C", n, Z_c);
		printf("Average C Kernel Execution Time (%d runs): %lf ms\n", RUNS_PER_VERSION, avg_time_c);


		//=== x86-64 Kernel Execution and Timing ===
		for (int run = 0; run < RUNS_PER_VERSION; run++) {
			start_ticks = get_time_ticks();
			// FROM Instruction: time the kernel portion only
			distance_kernel_asm(n, X1, X2, Y1, Y2, Z_asm);
			end_ticks = get_time_ticks();
			total_time_asm += (double)(end_ticks - start_ticks);
		}
		double avg_time_asm = total_time_asm * microseconds_per_tick / RUNS_PER_VERSION / 1000.0; //to milliseconds
		printf("\n [o] x86-64 Kernel Complete.\n");
		print_first_ten_elements("x86-64", n, Z_asm);
		printf("Average x86-64 Kernel Execution Time (%d runs): %lf ms\n", RUNS_PER_VERSION, avg_time_asm);

		check_correctness(n, Z_c, Z_asm);
		printf("\n--- Comparative Execution Time Analysis (N = %ld) ---\n", n);
		printf("      C = %lf ms \n", avg_time_c);
		printf(" x86-64 = %lf ms \n", avg_time_asm);

		if (avg_time_c > avg_time_asm) {
			printf("Analysis: x86-64 kernel is %.2f times FASTER than C kernel.\n", avg_time_c / avg_time_asm);
		}
		else {
			printf("Analysis: x86-64 kernel is %.2f times SLOWER than C kernel.\n", avg_time_asm / avg_time_c);
		}

		cleanup:
			X1 = X2 = Y1 = Y2 = Z_c = Z_asm = NULL;
			free(X1);
			free(X2);
			free(Y1);
			free(Y2);
			free(Z_c);
			free(Z_asm);
	}


		

	return 0;
}