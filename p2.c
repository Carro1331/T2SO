#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

sem_t wrt;
pthread_mutex_t mutex;
int pergamino1 = 1;
int pergamino2 = 1;
int lectores=0;
int escritores=0;
int randomnumber;


void *lectura(void *leer)
{   
    bool val = true;
    int emergency = 0; 
    
    while(val)
    {
        sem_wait(&wrt);
        lectores++;
        if(lectores == 1) 
        {
            pthread_mutex_lock(&mutex);
        }
        sem_post(&wrt);
    
        printf("Lector %d Lee el pergamino1: %d\n",*((int *)leer),pergamino1); //Info pergamino 1
        sem_wait(&wrt);
        lectores--;
        if(lectores == 0) 
        {
            pthread_mutex_unlock(&mutex); /* Si es el último lector, sale de la zona*/
        }
        sem_post(&wrt);
        sleep(3);
        emergency++;
        if(emergency >= 100) {break;}
    }
}


void *escritura(void *escribir)
{   
    int ingresopergamino1 = 0;
    srand(time(NULL));
    bool val = true;
    int emergency = 0;
    while(val)
    {
        ingresopergamino1 = ingresopergamino1+rand() % 11; //entonce

        if(ingresopergamino1 == 0)
        {
            pthread_mutex_lock(&mutex);
            
            pergamino1=pergamino1*11;
            
            if((pergamino1 == 0 || pergamino2 == 0)|| (pergamino1 < 0) || (pergamino2 < 0))
            {
                continue;
            }
            printf("Escritor %d escribe en el pergamino 1: %d\n",(*((int *)escribir)), pergamino1); 
            pthread_mutex_unlock(&mutex);
        }
        else
        {
            pthread_mutex_lock(&mutex);
            pergamino2=pergamino2*12;
            if((pergamino1 == 0 || pergamino2 == 0)|| (pergamino1 < 0) || (pergamino2 < 0))
            {
                continue;
            }
            printf("Escritor %d escribe en el pergamino 2: %d\n",(*((int *)escribir)), pergamino2); 
            pthread_mutex_unlock(&mutex);
        }
        emergency++;
        if(emergency >= 100) {break;}
    }
    sleep(3);
}


int main()
{   
    int cantidadlectores;
    scanf("%d",&cantidadlectores);
    pthread_t lee[1];
    pthread_t escribe[2];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&wrt,0,1);

    int info[cantidadlectores]; 
    for(int i = 0; i < cantidadlectores; i++) // se crean los hilos por lector
    {
        info[i]=i;
        pthread_create(&lee[i], NULL, (void *)lectura, (void *)&info[i]);
    }
    for(int i = 0; i < 2; i++) //se crean los hilos para cada escritor
    {
        pthread_create(&escribe[i], NULL, (void *)escritura, (void *)&info[i]);
    }
    for(int i = 0; i < cantidadlectores; i++) //espera y entra 
    {
        pthread_join(lee[i], NULL);
    }
    for(int i = 0; i < 2; i++) 
    {
        pthread_join(escribe[i], NULL);
    }
    pthread_mutex_destroy(&mutex); //acaba el mutex
    sem_destroy(&wrt); //acaba el semáforo

    return 0;
    
}
