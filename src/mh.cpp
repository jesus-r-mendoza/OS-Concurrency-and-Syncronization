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
int beg=1,day=1;
sem_t mo;
sem_t fa;
pthread_attr_t attr;


void init()
{
	sem_init(&mo,0,1);
	sem_init(&fa,0,0);
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
}

void *mother (void *args){
	int i;

	while(beg<=days)
	{
	   printf("\n\n This is day :\t%d  of a day in the life of Mother Hubbard \n",beg);
		for(i =0; i < 12 ; i++)
					{
						printf(" Mother wakes the child #%d up\n",i+1);
					}
				for(i =0; i < 12 ; i++)
				{
					printf("Mother gives breakfast to child #%d \n",i+1);
				}
				for(i =0; i < 12 ; i++)
					{
						printf("Mother sends the child #%d to school\n",i+1);
					}
				for(i =0; i < 12 ; i++)
					{
						printf("Mother  gives dinner to child #%d\n",i+1);
					}
				for(i =0; i < 12 ; i++)
					{
					printf("Mother gives bath to child #%d\n",i+1);
					usleep(100);
					sem_post(&fa);//Mother wakes the father up
					}
				beg++;
	}
}

void *father(void *args){
	int i;

	while(day<=days){
		printf("\n\nFather has woken up for day :\t%d\n\n",day);
		for(i =0; i < 12 ; i++)
				{
					sem_wait(&fa); //Father is waiting to be woken up
					printf("Father reads the child #%d a book\n",i+1);
					usleep(100);
				}

				for(i =0; i < 12 ; i++)
					{

						printf("Father makes the child #%d sleep\n",i+1);
					}

				printf("\n\nFather wakes the Mother and goes to sleep\n");
		day++;
		sem_post(&mo);
	}
}




int main(int argc, char **argv) {

	if (argc != 2)//checking for number of arguments
	{
		printf("\nHey argument must be 2!!");
		printf("\nThe correct format is ./mh <# of Days>\n");

	} else {

		printf("\nYou have given correct number of Arguments\n");
		days = atoi(argv[1]);
		printf("\n Number of Days\t =\t%d\n\n", days);

		init();

		pthread_t m;
		pthread_t f;

		pthread_create(&m,&attr,mother,NULL);
		pthread_create(&f,&attr,father,NULL);

		pthread_join(m,NULL);
		pthread_join(f,NULL);

	exit(0);

	}
}


