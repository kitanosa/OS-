#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include<time.h>
#include<sys/time.h>
 
char* sieve;
int num;
 
// функция потока
// int это инт для больших чисел
void* sieve_step(void* i_void) {// единицами все числа, кратные i. функция решето. Ставит "1" в массиве там где точно не простое число. маркировка чисел кратных i
    int i = *(int*)i_void;
    for (int j = i; j <= num; j += i) {
        sieve[j] = 1;
    }
    pthread_exit(NULL);
}
 
int main(int argc, char* argv[]) {// подсчет argc идет с единицы
    clock_t begin = clock();// начало отсчета времени программы
    if (argc != 3) {
        printf("Syntax: ./out Number_of_threads Number_for_test\n");
        exit(1);
    }
 
    int threads_num = atoi(argv[1]);// количество потоков
    num = atoi(argv[2]); // число которое ввели с консоли
 
 
    pthread_t* threads = (pthread_t*)calloc(threads_num, sizeof(pthread_t)); // массив адресов потоков. pthread_t - хранит в себе адрес потока
    if (threads == NULL) {
        printf("Can't allocate space for threads\n");
        exit(2);
    }
 
    int* args = (int*)malloc(threads_num * sizeof(int));// массив в котором мы храним то число, кратные которому мы должны удалить. Их threads_num количество чтобы для каждого потока было
    if (args == NULL) {
        printf("Can't create an array for arguments for threads\n");
        exit(3);
    }
    

    sieve = (char*)calloc(num, sizeof(char));// массив, заполненный 0 для решета
    if (sieve == NULL) {
        printf("Can't create an array for sieve\n");
        exit(3);
    }
 
    // маркировка чисел, не являющихся простыми по определению 0 - простое число, 1 - непростое число
    sieve[0] = 1;
    sieve[1] = 1;
 
    int cur_thread = 0; // id текущего потока
    // 0 - простое число, 1 - непростое число
    /*
    как только мы нашли простое число, мы запускаем поток (pthread_create) и просим его вычеркнуть все кратные числа (это описано в функе sieve_step).
    когда нашли следующее простое число - просим следующий поток и так далее будем запускать все потоки по кольцевой очереди. но нужно учитывать что когда мы даем новое задание потоку,
    он может еще выполнять старое задание, поэтому нам нужно дождаться окончания его работы (pthread_join).
    в конце нам также нужно дождаться пока все потоки завершат работу, а потом смотрим на полученный массив и говорим, простое число или нет.
    */
    for (int i = 2; i * i <= num; ++i) {
        if (sieve[i] == 1) { // пропуск не простых чисел
            continue;
        }
        if (cur_thread >= threads_num) {
            // мы должны подождать, пока работает необходимый поток
            pthread_join(threads[cur_thread % threads_num], NULL);// ожидание завершения всех потоков
        }
 
        args[cur_thread % threads_num] = i; // cкопирование аргумента для функции потока в специальный массив
        pthread_create(&threads[cur_thread % threads_num], NULL, sieve_step, &args[cur_thread % threads_num]);// первый - id потока
        ++cur_thread;
    }
 
    // waiting for all threads
    for (int i = 0; i < threads_num; ++i) {
        pthread_join(threads[i], NULL);
    }
 
    if (sieve[num] == 1) {
        printf("%d is not a prime number\n", num);
    }
    else {
        printf("%d is a prime number\n", num);
    }


    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f\n", time_spent);

    free(sieve);
    free(threads);
    free(args);
}