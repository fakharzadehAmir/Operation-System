#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_CUSTOMERS 5 // number of customers
#define NUM_TRANSACTION 10 // number of transactions/threads
#define INIT_STOCK 100 // the initial value for each person stock

// type transaction. you should pass an object of this type to the function arguments while creating thread
typedef struct
{
    double amount;
    int customer_id;
} transaction_t;

double safeBox = 1000.0; // shared variable for all customers

///////// Start your code here ///////////

/*1. define an array to store the stock of all customers
  2. define the necessary mutexes for each customer account and safe box*/
double costumers_stock[NUM_CUSTOMERS];
pthread_mutex_t safe_mutex;
pthread_mutex_t costumer_mutex[NUM_CUSTOMERS];

void* withdraw(void* arg){
    /*be aware to print the result in each situation.
      for example in one case that the stock isn't adequate and customer took money from safe box, you can report like the following:
      Customer <customerID> withdrew <transaction-amount> and <the amount taken from safe box> from safe-box. New balance: <new amount of customer stock>\tsafe-box value: <new amount of safe box>\n
      don't forget to report the new balance and new safe box value*/
    transaction_t* tran = (transaction_t *) arg;
    pthread_mutex_lock(&costumer_mutex[tran->customer_id]);

    if(tran->amount > costumers_stock[tran->customer_id]) {
        double helpFund = costumers_stock[tran->customer_id] > 0 ? tran->amount - costumers_stock[tran->customer_id] : tran->amount;
        pthread_mutex_lock(&safe_mutex);
        safeBox -= helpFund;
        costumers_stock[tran->customer_id] -= tran->amount;
        printf("Customer %d withdrew %.2f and %.2f from safe-box. New balance: %.2f\tsafe-box value: %.2f\n",
               tran->customer_id, tran->amount
               , helpFund, costumers_stock[tran->customer_id], safeBox);
        pthread_mutex_unlock(&safe_mutex);
    } else {
        costumers_stock[tran->customer_id] -= tran->amount;
        printf("Customer %d withdrew %.2f. New balance: %.2f\n", tran->customer_id, tran->amount, costumers_stock[tran->customer_id]);
    }

    pthread_mutex_unlock(&costumer_mutex[tran->customer_id]);
    return NULL;

}

void* deposit(void* arg){
    /*be aware to print the result in the following form:
      Customer <customerID> stock was charged by <transaction-amount>. New balance: <new amount of customer stock>\tsafe-box value: <new amount of safe box>\n
      don't forget to report the new balance and new safe box value*/
    transaction_t* tran = (transaction_t *) arg;
    pthread_mutex_lock(&costumer_mutex[tran->customer_id]);

    if (costumers_stock[tran->customer_id] < 0) {
        double deficit = -costumers_stock[tran->customer_id];
        double safeBoxMaxDeposit = 1000.0 - safeBox;
        double actualDeposit = (tran->amount < safeBoxMaxDeposit) ? tran->amount : safeBoxMaxDeposit;

        if (actualDeposit > deficit) {
            tran->amount = actualDeposit - deficit;
        } else {
            tran->amount = 0;
        }

        pthread_mutex_lock(&safe_mutex);
        safeBox += actualDeposit;
        costumers_stock[tran->customer_id] += actualDeposit;
        printf("Customer %d stock was charged by %.2f from safe-box. New balance: %.2f\tsafe-box value: %.2f\n",
               tran->customer_id, actualDeposit, costumers_stock[tran->customer_id], safeBox);
        pthread_mutex_unlock(&safe_mutex);
    } else {
        double safeBoxMaxDeposit = 1000.0 - safeBox;
        double actualDeposit = (tran->amount < safeBoxMaxDeposit) ? tran->amount : safeBoxMaxDeposit;

        costumers_stock[tran->customer_id] += actualDeposit;
        printf("Customer %d stock was charged by %.2f. New balance: %.2f\tsafe-box value: %.2f\n",
               tran->customer_id, actualDeposit, costumers_stock[tran->customer_id], safeBox);
    }

    pthread_mutex_unlock(&costumer_mutex[tran->customer_id]);
    return NULL;
}


int main() {
    srand(time(NULL));

    // Initialize customer stocks and mutexes
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        costumers_stock[i] = INIT_STOCK;
        pthread_mutex_init(&costumer_mutex[i], NULL);
    }

    pthread_mutex_init(&safe_mutex, NULL);

    pthread_t threads[NUM_TRANSACTION];
    transaction_t transactions[NUM_TRANSACTION];

    // Create threads for transactions
    for (int i = 0; i < NUM_TRANSACTION; i++) {
        transactions[i].amount = (rand() % 100) + 1;
        transactions[i].customer_id = rand() % NUM_CUSTOMERS;

        if (rand() % 2 == 0) {
            pthread_create(&threads[i], NULL, withdraw, (void *)&transactions[i]);
        } else {
            pthread_create(&threads[i], NULL, deposit, (void *)&transactions[i]);
        }
    }

    for (int i = 0; i < NUM_TRANSACTION; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&safe_mutex);
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_mutex_destroy(&costumer_mutex[i]);
    }

    return 0;
}
