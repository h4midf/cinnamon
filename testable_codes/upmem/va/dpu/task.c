
#include <stdint.h>
#include <stdio.h>
#include <defs.h>
#include <mram.h>
#include <alloc.h>
#include <barrier.h>
#include <seqread.h>
#include <perfcounter.h>

// #include "../support/common.h"
// #include "../generated_headers/config.h"

// __host dpu_arguments_t DPU_INPUT_ARGUMENTS;
// __host uint32_t nb_cycle;


// Barrier
BARRIER_INIT(my_barrier, NR_TASKLETS);
BARRIER_INIT(my_barrier2, NR_TASKLETS);

// main
int main() {
	unsigned int tasklet_id = me();
	if (tasklet_id == 0){ // Initialize once the cycle counter
		mem_reset(); // Reset the heap
	}

	// Barrier
	barrier_wait(&my_barrier);
	barrier_wait(&my_barrier2);

	// uint32_t ITER_I  = DPU_INPUT_ARGUMENTS.ITER_I;
	// uint32_t ITER_J  = DPU_INPUT_ARGUMENTS.ITER_J;
	// uint32_t ITER_Z  = DPU_INPUT_ARGUMENTS.ITER_Z;
	// uint32_t THREAD_I = DPU_INPUT_ARGUMENTS.THREAD_I;
	// uint32_t THREAD_J = DPU_INPUT_ARGUMENTS.THREAD_J;

	// uint32_t tid_idx_i = tasklet_id / THREAD_I;
	// uint32_t tid_idx_j = tasklet_id % THREAD_I;


	// if ((tid_idx_i >= THREAD_I)) return 0;

	// uint32_t mram_base_addr_A = (uint32_t) (DPU_MRAM_HEAP_POINTER);
	// uint32_t mram_temp_addr_A = mram_base_addr_A + (tid_idx_i * ITER_I) * sizeof(T);
	// uint32_t A_BUFFER_SIZE = DPU_INPUT_ARGUMENTS.A_BUFFER_SIZE;
	// uint32_t B_BUFFER_SIZE = DPU_INPUT_ARGUMENTS.B_BUFFER_SIZE;
	// uint32_t C_BUFFER_SIZE = DPU_INPUT_ARGUMENTS.C_BUFFER_SIZE;
	// T *cache_A = (T *) mem_alloc(A_BUFFER_SIZE );
	// uint32_t a_offset = ITER_I * ITER_J * THREAD_I * THREAD_J * sizeof(T);
	// uint32_t mram_base_addr_B = (uint32_t) (DPU_MRAM_HEAP_POINTER) + a_offset;
	// uint32_t mram_temp_addr_B = mram_base_addr_B + (tid_idx_i * ITER_I) * sizeof(T);
	// T *cache_B = (T *) mem_alloc(B_BUFFER_SIZE);
	// // T *cache_C = (T *) mem_alloc(B_BUFFER_SIZE);

	// for (int r = 0; r < ITER_I; r++) {
	// 	mram_read((__mram_ptr void const*) (mram_temp_addr_A), cache_A, A_BUFFER_SIZE);
	// 	mram_read((__mram_ptr void const*) (mram_temp_addr_B), cache_B, B_BUFFER_SIZE);
	// 	mram_temp_addr_A += A_BUFFER_SIZE;
	// 	mram_temp_addr_B += B_BUFFER_SIZE;
	// 	for (int c = 0; c < ITER_J; c++) {
	// 		cache_A[c] = cache_A[c] + cache_B[c];
	// 	}
	// 	mram_write(cache_A, (__mram_ptr void *) (mram_temp_addr_A), A_BUFFER_SIZE);
	// }


	// #ifdef SIM
	// #if SIM == 1
    // nb_cycle = perfcounter_get();	
	// if(me() == 0){
	// 	printf("cycle %d\n", nb_cycle);
	// 	// printf("ITER_I %d ITER_J %d ITER_Z %d\n", ITER_I, ITER_J, ITER_Z);
	// 	// printf("X %d\n", x);
	// 	// printf("ITER_I %d cycle %d %d %d\n", ITER_I, nb_cycle, x, y);
	// }	
	// #endif
	// #endif

	// #ifdef LOG
	// #if LOG == 1
	// if(me() == 0){
	// 	// printf("iter %d\n", ITER_Z);
	// 	// printf("ITER_I %d cycle %d %d %d\n", ITER_I, nb_cycle, x, y);
	// }	
	// #endif
	// #endif

	return 0;
}