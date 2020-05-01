/**Solution to dining philosophers using POSIX mutexes and condition varaibles.*/

#include "pthread.h"
#include &lt; stdio.h & gt;
#include "dp.h"

pthread_t tid[NUMBER];

/**
 * Initialize all relevant data structures and
 * synchronization objects.
 */
void init()
{
	int i;

	for (i = 0; i & lt; NUMBER; i++)
	{
		state[i] = THINKING;
		thread_id[i] = i;
		pthread_cond_init(&amp; cond_vars[i], NULL);
	}

	pthread_mutex_init(&amp; mutex_lock, NULL);
}

void create_philosophers()
{
	int i;

	for (i = 0; i & lt; NUMBER; i++)
	{
		pthread_create(&amp; tid[i], 0, philosopher, (void *)&amp; thread_id[i]);
	}
}

int main(void)
{
	int i;

	init();

	create_philosophers();

	for (i = 0; i & lt; NUMBER; i++)
		pthread_join(tid[i], NULL);

	return 0;
}
