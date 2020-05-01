#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define N_PHILOSOPHERS 5
#define LEFT (ph_num + N_PHILOSOPHERS - 1) % N_PHILOSOPHERS
#define RIGHT (ph_num + 1) % N_PHILOSOPHERS
#define MAX_MEALS 10
#define MAX_WAIT 3

pthread_mutex_t mutex;
pthread_cond_t condition[N_PHILOSOPHERS];

enum { THINKING, HUNGRY, EATING } state[N_PHILOSOPHERS];
int phil_num[N_PHILOSOPHERS];

void *philosopher (void *arg);
void pickup_chopsticks(int ph_num);
void return_chopsticks(int ph_num);
void test(int ph_num);

int main(int argc, char *argv[])
{
  if(argc != 2)
  {
    printf("expected runtime");
    return -1;
  }

  int runtime = atoi(argv[1]);

  /* Setup */
  pthread_t ph_thread[N_PHILOSOPHERS];
  pthread_mutex_init(&mutex, NULL);

  for (int i = 0; i < N_PHILOSOPHERS; i++) {
    pthread_cond_init(&condition[i], NULL);
    phil_num[i] = i;
  }

  /* Meat */
  for (int i = 0; i < N_PHILOSOPHERS; i++) {
    pthread_create(&ph_thread[i], NULL, philosopher, &phil_num[i]);
    printf("Philosopher #%d sits at the table.\n", i + 1);
  }

  sleep(runtime);
  
  for (int i = 0; i < N_PHILOSOPHERS; i++)
    pthread_cancel(ph_thread[i]);
  

  /* Cleanup */
  pthread_mutex_destroy(&mutex);
  for (int i = 0; i < N_PHILOSOPHERS; i++)
    pthread_cond_destroy(&condition[i]);

  return(0);
}

int checkTime(){
  
}

void think(int num){
  double range = (3.0-1.0);
  double div = RAND_MAX/range;
  double interval = 1.0+(rand()/div);
  printf("Philosopher #%d is THINKING.\n", num+1);
  sleep(interval);
}

void eat(int num){
  double range = (3.0-1.0);
  double div = RAND_MAX/range;
  double interval = 1.0+(rand()/div);
  printf("Philosopher #%d is EATING.\n", num+1);
  sleep(interval);
}

void *philosopher(void *arg)
{
  int meals = 0;
  int *ph_num = arg;
  while(1) {
    think(*ph_num);
    pickup_chopsticks(*ph_num);
    return_chopsticks(*ph_num);
    meals++;
    printf("Philosopher #%d ate: %d/%d meals\n", *ph_num + 1, meals, MAX_MEALS);
    if(meals == MAX_MEALS)
      break;
  }
  printf("Philosopher #%d is Full\n", *ph_num + 1);
}

void pickup_chopsticks(int ph_num) {
  pthread_mutex_lock(&mutex);

  printf("Philosopher #%d is HUNGRY.\n", ph_num + 1);
  state[ph_num] = HUNGRY;
  test(ph_num);
  while (state[ph_num] != EATING) 
    pthread_cond_wait(&condition[ph_num], &mutex);

  pthread_mutex_unlock(&mutex);
}

void return_chopsticks(int ph_num) {
  pthread_mutex_lock(&mutex);

  printf("Philosopher #%d returns chopsticks.\n", ph_num + 1);
  state[ph_num] = THINKING;
  test(LEFT);
  test(RIGHT);

  pthread_mutex_unlock(&mutex);
}

void test(int ph_num) {
  if (state[ph_num] == HUNGRY && 
      state[LEFT] != EATING && 
      state[RIGHT] != EATING) {
    eat(ph_num);
    state[ph_num] = EATING;
    pthread_cond_signal(&condition[ph_num]);
  }
}
