#include <pthread.h>
#include <stdio.h>

// gcc 4.7.2 +
// gcc -lpthread -std=gnu99 -Wall -g -o ex2_c ex2.c



int i = 0;
pthread_mutex_t lock;



void *thread_1_func(){

	for (int c = 0; c < 1000000; c++){
		pthread_mutex_lock(&lock);
		i++;
		pthread_mutex_unlock(&lock);
	}
	return 0;
}

void *thread_2_func(){
	for (int c = 0; c < 1000001; c++){
		pthread_mutex_lock(&lock);
		i--;
		pthread_mutex_unlock(&lock);
	}

	return 0;
}

int main(){
	pthread_t thread_1;
	pthread_t thread_2;

	if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

	pthread_create(&thread_1, NULL, thread_1_func, NULL);
	pthread_create(&thread_2, NULL, thread_2_func, NULL);

	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);

	printf("i = %i \n", i);


	return 0;

}

