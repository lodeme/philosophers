#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>

#define PHILOSOPHERS 3

void philosopher_actions(int id, sem_t *forks_semaphore);

int main() {
	sem_t *forks_semaphore;
	int i;
	pid_t pid;

	// Initialize a single semaphore to represent all forks
	sem_unlink("forks_sem");
	forks_semaphore = sem_open("forks_sem", O_CREAT, 0644, PHILOSOPHERS);
	if (forks_semaphore == SEM_FAILED) {
		perror("sem_open");
		exit(EXIT_FAILURE);
	}

	// Create philosopher processes
	for (i = 0; i < PHILOSOPHERS; i++) {
		pid = fork();
		if (pid < 0) {
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0) {
			philosopher_actions(i, forks_semaphore);
			exit(EXIT_SUCCESS);
		}
	}

	// Wait for philosopher processes to finish
	for (i = 0; i < PHILOSOPHERS; i++) {
		wait(NULL);
	}

	// Cleanup semaphore
	sem_close(forks_semaphore);
	sem_unlink("forks_sem");

	return 0;
}

void philosopher_actions(int id, sem_t *forks_semaphore) {
	while (1) {

		if (id % 2 == 0)
			sleep(1);
		printf("Philosopher %d is thinking.\n", id);
		sem_wait(forks_semaphore); // Attempt to take a fork
		printf("Philosopher %d took a fork.\n", id);
		sem_wait(forks_semaphore); // Attempt to take a fork
		printf("Philosopher %d took a fork.\n", id);
		printf("Philosopher %d is eating.\n", id);
		sleep(1); // Simulate eating
		sem_post(forks_semaphore); // Put down the fork
		sem_post(forks_semaphore); // Put down the fork
		printf("Philosopher %d is sleeping.\n", id);
		sleep(1);
	}
}
