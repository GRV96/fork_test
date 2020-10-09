#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "process.h"

void print_fork_call_num(int fork_calls);

void print_proc_tree(const Process* processes,
		int generation, int proc_count);

int main() {
	int fork_calls = 0;
	pid_t pid;
	int proc_num = 0;
	const pid_t initial_pid = getpid();

	printf("Inital PID: %d\n", initial_pid);

	Process initial_proc;
	init_process(&initial_proc, initial_pid, fork_calls);
	Process* parent = &initial_proc;

	pid = fork();
	if(pid < 0) {
		printf("Error! Process creation failed.\n");
		return EXIT_FAILURE;
	}
	fork_calls++;

	if(pid > 0) {
		Process* child = (Process*) malloc(sizeof(Process));
		if(child == NULL) {
			printf("Error! Memory allocation failed.\n");
			return EXIT_FAILURE;
		}
		init_process(child, pid, fork_calls);
		proc_add_child(parent, child);
		parent = child;
	}

	if(getpid() != initial_pid) {
		// All processes but the initial process end here.
		exit(0);
	}

	if(proc_num-1 < 2) {
		printf("%d process was created.\n", proc_num+1);
	}
	else {
		printf("%d processes were created.\n", proc_num+1);
	}
	printf("fork_calls: %d\n", fork_calls);

	Process p[] = {initial_proc};
	print_proc_tree(p, 0, 1);
	proc_free_child_mem(&initial_proc);

	return EXIT_SUCCESS;
}

void print_proc_tree(const Process* processes,
		int generation, int proc_count) {
	printf("Generation %d\n\t", generation);

	int i;
	for(i=0; i<proc_count; i++) {
		pid_t pid = processes[i].pid;
		int fc = processes[i].fork_calls;
		pid_t ppid = -1;
		if(processes[i].parent != NULL) {
			ppid = processes[i].parent->pid;
		}
		printf("[fc: %d, pid: %d, ppid: %d] ", fc, pid, ppid);
	}
	printf("\n");

	for(i=0; i<proc_count; i++) {
		if(processes[i].children != NULL) {
			print_proc_tree(processes[i].children, generation+1,
					processes[i].child_count);
		}
	}
}

void print_fork_call_num(int fork_calls) {
	if(fork_calls < 2) {
		printf("%d fork call\n", fork_calls);
	}
	else {
		printf("%d fork calls\n", fork_calls);
	}
}
