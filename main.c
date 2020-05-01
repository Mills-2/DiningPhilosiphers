#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>


#define PHILOSOPHER_NUM 5
#define MAX_MEALS 10

pthread_t philosophers[PHILOSOPHER_NUM];
pthread_mutex_t mutex_forks = PTHREAD_MUTEX_INITIALIZER;;
int forks[PHILOSOPHER_NUM];

double MAX_THINK_EAT_SEC = 3.0;

double patience() {
	double gen = (double)rand() / RAND_MAX;
	return 1.0 + gen * (MAX_THINK_EAT_SEC - 1.0);
}

void init() {
	int i;
	for(i=0; i<PHILOSOPHER_NUM; i++)
	forks[i] = 0;
}

void* philosopher(void *args) {
	int i = (intptr_t)args;
	int right = i;
	int left = (i - 1 == -1) ? PHILOSOPHER_NUM - 1 : (i - 1);
	int locked;
	int meals = 0;
	time_t endwait;
	int seconds = 10;
	endwait = time (NULL) + seconds;
	while((time (NULL) < endwait) && meals < MAX_MEALS) {
		locked = 0;
		while(!locked) {
			pthread_mutex_lock(&mutex_forks);
			if(forks[right] || forks[left]) {
				pthread_mutex_unlock(&mutex_forks); // give up the forks unless you can take both at once
				printf("Philosopher %d cannot take forks. Giving up and thinking.\n",i);
				sleep(patience()); // think.
				continue;
			}
			forks[right] = 1; // take forks.
			forks[left] = 1;
			pthread_mutex_unlock(&mutex_forks);
			locked = 1;
		}
	meals++;
	printf("Philosopher %d took both forks, now eating meal %d of %d.\n", i, meals, MAX_MEALS);
	sleep(patience());
	pthread_mutex_lock(&mutex_forks); // give up forks.
	forks[right] = 0;
	forks[left] = 0;
	pthread_mutex_unlock(&mutex_forks);
	sleep(patience());
	}
	printf("Philsopher %d ate %d meals in %d seconds.\n", i, meals, seconds);
}

int main(int argc, char* argv[]) {
	init();
	int j, k, inputSeconds;
	printf("enter a run time in seconds: ");
	scanf("%d", &inputSeconds);
	printf("run time is %s\n", argv[1]);
	for(j = 0; j < 5; j++) {
		pthread_create( &philosophers[j], NULL, philosopher, (void*) (intptr_t) j);
	}
	for(k = 0; k < 5; k++) {
		pthread_join(philosophers[k], NULL);
	}
	return 0;
}
