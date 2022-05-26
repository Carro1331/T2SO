#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

sem_t escrib;
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
        sem_wait(&escrib);     // Entra al semáforo
        lectores++;
        if(lectores == 1) 
        {
            pthread_mutex_lock(&mutex); // Iniciar Mutex (Si es el primer lector) Sección Crítica
        }
        sem_post(&escrib);     // Cierra el semáforo
    
        printf("Lector %d Lee el pergamino1: %d\n",*((int *)leer),pergamino1); //Info pergamino 1
        sem_wait(&escrib);     // Sale del semáforo
        lectores--;
        if(lectores == 0) 
        {
            pthread_mutex_unlock(&mutex); // Cierra Mutex (si es el último lector) Sección Crítica
        }
        sem_post(&escrib);
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
        ingresopergamino1 = ingresopergamino1+rand() % 11; // Entonces uwu

        if(ingresopergamino1 == 0)
        {
            pthread_mutex_lock(&mutex); // Inicia el mutex (Sección Crítica)
            
            pergamino1=pergamino1*11;
            
            if((pergamino1 == 0 || pergamino2 == 0)|| (pergamino1 < 0) || (pergamino2 < 0))
            {
                continue;
            }
            printf("Escritor %d escribe en el pergamino 1: %d\n",(*((int *)escribir)), pergamino1); 
            pthread_mutex_unlock(&mutex); // Acaba el mutex (Sección Crítica)
        }
        else
        {
            pthread_mutex_lock(&mutex); // Inicia el mutex (Sección Crítica)
            pergamino2=pergamino2*12;
            if((pergamino1 == 0 || pergamino2 == 0)|| (pergamino1 < 0) || (pergamino2 < 0))
            {
                continue;
            }
            printf("Escritor %d escribe en el pergamino 2: %d\n",(*((int *)escribir)), pergamino2); 
            pthread_mutex_unlock(&mutex); // Acaba el mutex (Sección Crítica)
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
    pthread_mutex_init(&mutex, NULL); // Crear mutex
    sem_init(&escrib,0,1);     // Crear semáforo

    int info[cantidadlectores]; 
    for(int i = 0; i < cantidadlectores; i++) // Se crean threads para el papel de lector
    {
        info[i]=i;
        pthread_create(&lee[i], NULL, (void *)lectura, (void *)&info[i]);
    }
    for(int i = 0; i < 2; i++) // Se crean threads para el papel de escritores
    {
        pthread_create(&escribe[i], NULL, (void *)escritura, (void *)&info[i]);
    }
    for(int i = 0; i < cantidadlectores; i++) // Espera y entra cada threads 
    {
        pthread_join(lee[i], NULL);
    }
    for(int i = 0; i < 2; i++)              // Espera y entra cada threads
    {
        pthread_join(escribe[i], NULL);
    }
    pthread_mutex_destroy(&mutex); // Acaba el mutex
    sem_destroy(&escrib); // Acaba el semáforo

    return 0;
}

//Mientras los lectores están ingresando a un pergamino, este no puede ser editado, tampoco pueden ingresar al otro pergamino,
//ya que este en paralelo debe estar recibiendo alguna escritura. 
//Por ende, el resultado debería ser que se edite un pergamino mientras el otro está siendo leido por todos los lectores, 
//al finalizar la lectura del último lector, el otro pergamino puede ser leido por los lectores, mientras que el anterior pergamino
//debe estar recibiendo alguna escritura del escritor.
