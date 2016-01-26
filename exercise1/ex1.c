#include <pthread.h>
#include <stdio.h>


int i = 0;

void *thread_1_func(){
	for (int c = 0; c < 1000000; c++){
		i++;
	}
	return 0;
}

void *thread_2_func(){
	for (int c = 0; c < 1000000; c++){
		i--;
	}
	return 0;
}

int main(){
	pthread_t thread_1;
	pthread_t thread_2;

	pthread_create(&thread_1, NULL, thread_1_func, NULL);
	pthread_create(&thread_2, NULL, thread_2_func, NULL);

	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);

	printf("i = %i \n", i);


	return 0;

}

