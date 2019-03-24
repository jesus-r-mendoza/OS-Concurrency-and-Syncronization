#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/time.h>

int days;
int beg = 1, day = 1;
sem_t mo;
sem_t fa;
pthread_attr_t attr;

void init() // start the semaphores
{
	sem_init(&mo, 0, 1);
	sem_init(&fa, 0, 0);
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
}

void *mother(void *args)
{ //Mother conducts the task on her 12 childs ,when completed wakes Father up and Mother goes to sleep.
	int i;

	while (beg <= days)
	{
		printf("\n\n This is day :\t%d  of a day in the life of Mother Hubbard \n", beg);
		for (i = 0; i < 12; i++)
		{
			printf("child #%d is woken up by Mother\n", i + 1);
		}
		for (i = 0; i < 12; i++)
		{
			printf("child #%d is being fed breakfast by mother \n", i + 1);
		}
		for (i = 0; i < 12; i++)
		{
			printf("child #%d is sent to school by Mother \n", i + 1);
		}
		for (i = 0; i < 12; i++)
		{
			printf("child #%d is given dinner by Mother\n", i + 1);
		}
		for (i = 0; i < 12; i++)
		{
			printf("child #%d is given a bath by Mother\n", i + 1);
			usleep(100);
			sem_post(&fa); //Mother wakes the father up
		}
		beg++;
	}
}

void *father(void *args)
{ //Father wakes up and complete 12 childs tasks, then wake up mother
	int i;

	while (day <= days)
	{
		printf("\n\nFather has woken up for day :\t%d\n\n", day);
		for (i = 0; i < 12; i++)
		{
			sem_wait(&fa); //Father is waiting to be woken up
			printf("child #%d is being read a book by father \n", i + 1);
			usleep(100);
		}

		for (i = 0; i < 12; i++)
		{

			printf("child #%d goes to sleep by the father\n", i + 1);
		}

		printf("\n\n Father is going to sleep and waking up Mother to take care of children \n");
		day++;
		sem_post(&mo);
	}
}

int main(int argc, char **argv)
{ //input the number of days the Mother & Father will conduct the tasks.

	if (argc != 2) //checking for the number of arguments
	{
		printf("\n argument must be 2");
		printf("\n format is ./mh <# of Days>\n");
	}
	else
	{

		days = atoi(argv[1]);
		printf("\n Number of Days\t =\t%d\n\n", days);
		// the threads are created and waited to join
		init();

		pthread_t m;
		pthread_t f;

		pthread_create(&m, &attr, mother, NULL);
		pthread_create(&f, &attr, father, NULL);

		pthread_join(m, NULL);
		pthread_join(f, NULL);

		exit(0);
	}
}