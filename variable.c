#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

// A global or local variable's value can change from a process to another.

int g = 19;

int main() {
	printf("Initial PID: %d\n", getpid());

	int x = 7;

	pid_t pid = fork();
	if(pid < 0) {
		printf("Error! Process creation failed.");
		return EXIT_FAILURE;
	}
	else if(pid == 0) { // Child process
		g = 23;
		x = 11;
	}
	else { // Initial process
		int status;
		wait(&status);
	}

	printf("In process %d, x = %d and g = %d.\n", getpid(), x, g);
	/* Console output
	Initial PID: [initial PID]
	In process [child PID], x = 11 and g = 23.
	In process [initial PID], x = 7 and g = 19.
	*/

	return EXIT_SUCCESS;
}
