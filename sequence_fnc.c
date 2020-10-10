#include <stdlib.h>

#include "process.h" // Includes sys/types.h.

int create_process(int* fork_calls, Process* current_proc);

void print_fork_call_count(int fork_calls);

void print_proc_ancestry(const Process* process);

void print_proc_count(int proc_count);

int main() {
	int fork_calls = 0;
	pid_t pid;
	const pid_t initial_pid = getpid();

	printf("Initial PID: %d\n", initial_pid);

	Process initial_proc;
	Process* proc = &initial_proc;
	init_process(proc, initial_pid, fork_calls);

	create_process(&fork_calls, proc);
	create_process(&fork_calls, proc);
	create_process(&fork_calls, proc);

	Process* oldest_ancestor = proc_oldest_ancestor(proc);
	if(oldest_ancestor != NULL) {
		proc_free_descendant_mem(oldest_ancestor);
	}

	return EXIT_SUCCESS;
}

int create_process(int* fork_calls, Process* current_proc) {
	pid_t pid = fork();
	if(pid < 0) {
		printf("Error! Process creation failed.\n");
		return -1;
	}
	(*fork_calls)++;

	if(pid == 0) {
		Process* child = (Process*) malloc(sizeof(Process));
		if(child == NULL) {
			printf("Error! Memory allocation failed.\n");
			return -1;
		}

		init_process(child, getpid(), *fork_calls);
		proc_add_child(current_proc, child);

		printf("Ancestors of process %d:\n", child->pid);
		print_proc_ancestry(child);

		current_proc = child;
	}
	return 0;
}

void print_fork_call_count(int fork_calls) {
	if(fork_calls < 2) {
		printf("%d fork call\n", fork_calls);
	}
	else {
		printf("%d fork calls\n", fork_calls);
	}
}

void print_proc_ancestry(const Process* process) {
	Process* proc = process;
	while(proc != NULL) {
		Process* parent = proc->parent;
		pid_t pid = proc->pid;
		int fc = proc->fork_calls;
		pid_t ppid = -1;
		if(parent != NULL) {
			ppid = parent->pid;
		}
		printf("[fc: %d, pid: %d, ppid: %d] ", fc, pid, ppid);
		proc = parent;
	}
	printf("\n");
}

void print_proc_count(int proc_count) {
	if(proc_count < 2) {
		printf("%d process was created.\n", proc_count);
	}
	else {
		printf("%d processes were created.\n", proc_count);
	}
}
