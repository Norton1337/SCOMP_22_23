#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

int is_prime(int n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    for(int i = 2; i * i <= n; i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}


void* print_primes(void *param){
    int limit = *((int*) param);
    for (int i = 2; i <= limit; i++) {
        if (is_prime(i)) {
            printf("%d\n", i);
        }
    }
    return NULL;
}

int main(){
    int user_number;
    printf("Insert a number to know his primes xDDD(positive pls): ");
    scanf("%d", &user_number);
    pthread_t thread;
    pthread_create(&thread, NULL, print_primes, (void *)&user_number);
    pthread_join(thread, NULL);
    return 0;   
    
}