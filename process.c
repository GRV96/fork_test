#include <stddef.h>

#include "process.h"

void init_process(Process* process, pid_t pid, int fork_calls) {
	process->pid = pid;
	process->fork_calls = fork_calls;
	process->parent = NULL;
	process->children = NULL;
	process->child_count = 0;
}

int proc_add_child(Process* process, Process* child) {
	child->parent = process;

	process->children = realloc(process->children,
			sizeof(Process)*(process->child_count+1));
	if(process->children == NULL) {
		return 1;
	}
	process->children[process->child_count] = *child;
	process->child_count++;

	return 0;
}

int proc_count_grandchildren(Process* process) {
	if(process->children == NULL) {
		return 0;
	}

	int grandchild_count = 0;
	int i;
	for(i=0; i<process->child_count; i++) {
		grandchild_count += process->children[i].child_count;
	}

	return grandchild_count;
}

void proc_free_child_mem(Process* process) {
	if(process == NULL) {
		return;
	}
	else if(process->children == NULL) {
		return;
	}
	else if(proc_count_grandchildren(process) == 0) {
		free(process->children);
		return;
	}

	int i;
	for(i=0; i<process->child_count; i++) {
		proc_free_child_mem(process->children+i);
	}
}

Process* proc_oldest_ancestor(const Process* process) {
	Process* proc = process;
	while(1) {
		Process* parent = proc->parent;
		if(parent == NULL) {
			return proc;
		}
		proc = parent;
	}
	return NULL;
}
