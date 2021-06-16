#include <math.h>
#include <stdio.h>

#include "scheduling_sim.h"
#include "scheduling_utility.h"

// ---------------------------------------------------------------------------
// Example schedulers
// ---------------------------------------------------------------------------

void fcfs(scheduler_context_t* ctx) {
	process_t* selected = get_process(ctx, 0);
	schedule_process(ctx, selected);
}

void round_robin(scheduler_context_t* ctx) {
	process_t* selected = get_process(ctx, 0);
	schedule_process(ctx, selected);
	move_process_to_end(ctx, selected);
}

void srtf(scheduler_context_t* ctx) {
	process_t* process_to_schedule = NULL;
	for (size_t i = 0; i < get_num_processes(ctx); i++) {
		process_t* in_queue = get_process(ctx, i);
		if (process_to_schedule == NULL ||
		    process_to_schedule->remaining_time > in_queue->remaining_time) {
			process_to_schedule = in_queue;
		}
	}
	schedule_process(ctx, process_to_schedule);
}

void round_robin_q4(scheduler_context_t* ctx) {
	process_t* selected = get_process(ctx, 0);
	// schedule the process
	schedule_process(ctx, selected);
	// if the process just arrived
	if (selected->service_time == selected->remaining_time) return;
	// if the process is already three timestamps active
	if ((selected->service_time - selected->remaining_time) % 3 == 0)
		move_process_to_end(ctx, selected);
}

void hpf_p(scheduler_context_t* ctx) {
	process_t* selected = NULL;
	for (size_t i = 0; i < get_num_processes(ctx); i++) {
		process_t* in_queue = get_process(ctx, i);
		if (selected == NULL || selected->priority < in_queue->priority) {
			selected = in_queue;
		}
	}
	schedule_process(ctx, selected);
}

void hpf_np(scheduler_context_t* ctx) {
	process_t* p = get_process(ctx, 0);
	if (p->remaining_time > 0) {
		schedule_process(ctx, p);
	}
	process_t* next_up = NULL;
	for (size_t i = 1; i < get_num_processes(ctx); i++) {
		process_t* in_queue = get_process(ctx, i);
		if (next_up == NULL) {
			next_up = in_queue;
			continue;
		}
		if (next_up->priority < in_queue->priority) {
			move_process_to_end(ctx, next_up);
		}
	}
}

// ---------------------------------------------------------------------------
// Implement your schedulers here
// ---------------------------------------------------------------------------

void my_schedule(scheduler_context_t* ctx) {
	process_t* selected = NULL;
	for (size_t i = 0; i < get_num_processes(ctx); i++) {
		process_t* in_queue = get_process(ctx, i);
		if (selected == NULL || selected->priority < in_queue->priority) {
			selected = in_queue;
		}
	}
	schedule_process(ctx, selected);
	if ((selected->service_time - selected->remaining_time) % 2 == 0)
		move_process_to_end(ctx, selected);
}

// ---------------------------------------------------------------------------

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "Error: Usage: %s <filename>\nExample: %s input.csv\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}

	simulation_data_t* data = read_simulation_data(argv[1]);
	if (data == NULL) {
		fprintf(stderr, "Error: Unable to open the file '%s' for reading!\n", argv[1]);
		return EXIT_FAILURE;
	}
	print_simulation_data(stdout, data);

	// -----------------------------------------------------------------------
	// Uncomment these calls to your schedulers
	// -----------------------------------------------------------------------

	// Examples
	print_schedule(stdout, compute_schedule(data, fcfs), data);
	print_schedule(stdout, compute_schedule(data, round_robin), data);

	// Task 1
	print_schedule(stdout, compute_schedule(data, srtf), data);
	print_schedule(stdout, compute_schedule(data, round_robin_q4), data);

	// Task 2
	print_schedule(stdout, compute_schedule(data, hpf_p), data);
	print_schedule(stdout, compute_schedule(data, hpf_np), data);

	// Task 3
	print_schedule(stdout, compute_schedule(data, my_schedule), data);

	// -----------------------------------------------------------------------

	free_simulation_data(data);

	return EXIT_SUCCESS;
}
