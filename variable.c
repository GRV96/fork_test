#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
	printf("Initial PID: %d\n", getpid());
	int x = 7;

	pid_t pid = fork();
	if(pid < 0) {
		printf("Error! Process creation failed.");
		return EXIT_FAILURE;
	}
	else if(pid == 0) { // Child process
		x = 11;
	}
	else { // Initial process
		int status;
		wait(&status);
	}

	printf("In process %d, x = %d.\n", getpid(), x);
	/* Console output
	Initial PID: [initial PID]
	In process [child PID], x = 11.
	In process [initial PID], x = 7.
	*/

	return EXIT_SUCCESS;
}
