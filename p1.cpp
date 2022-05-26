#include <iostream>
#include <queue>
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <queue>
#include<string>
#include<fstream>

using namespace std;

class Ramo {
public:
    int id, burst, diaLim, horaLim, deadline;
	Ramo(int burst, int diaLim, int horaLim, int deadline, int id)
		: burst(burst), diaLim(diaLim), horaLim(horaLim), deadline(deadline), id(id)
	{}
};

bool operator<(const Ramo& ram1, const Ramo& ram2){
	return ram1.deadline > ram2.deadline;
}

int transformacion(int dia, int hora){
    int limittime = dia*24 + hora;
    return limittime;
}

int counter;
pthread_mutex_t lock;
priority_queue<Ramo> Ramos;

int primero = 0;
int dias2 = 0; 
int horas2 = 8;
    
ofstream arc;
    


string diaSemana(int *num)
{
    string diastext;
    if(dias2 == 0)
        {diastext = "Lunes";}
    else if(dias2 == 1)
        {diastext = "Martes";}
    else if(dias2 == 2)
        {diastext = "Miercoles";}
    else if(dias2 == 3)
        {diastext = "Jueves";}
    else if(dias2 == 4)
        {diastext = "Viernes";}
    else if(dias2 == 5)
        {diastext = "Sabado";}
    else if(dias2 == 6)
        {diastext = "Domingo";}
    return diastext;
}

void* ThreadSpace(void *arg)
{

    pthread_mutex_lock(&lock);
    //arc.open("texto.txt", ios::out); 
    unsigned long i = 0;

    for(i=0; i<(0xFFFFFFF);i++);

    Ramo ram = Ramos.top();
	Ramos.pop();
    int resto, entero;
    int horas = 0;
    int dias = 0;
    if(ram.deadline >= 24)
    {
        resto = ram.deadline%24;
        entero = ram.deadline/24; 
    }
    if(resto == 0)
    {
        //printf("Ramo n°:%d || Burst: %d || Deadline -> Dias: %d || Hrs Totales : %d\n",ram.id, ram.burst,entero,ram.deadline); 
        cout <<"Ramo n°:" << ram.id <<" || Burst: "<< ram.burst <<" || Deadline -> Dias: " << entero<<"|| Hrs Totales : "<< ram.deadline << endl; 
        arc <<"Ramo n°:" << ram.id <<" || Burst: "<< ram.burst <<" || Deadline -> Dias: " << entero<<"|| Hrs Totales : "<< ram.deadline << endl; 
    }
    else
    {
        //printf("Ramo n°:%d || Burst: %d || Deadline -> Dias: %d a las %d hrs || Hrs Totales : %d\n",ram.id, ram.burst,entero,resto, ram.deadline);         
        cout <<"Ramo n°:" << ram.id <<" || Burst: "<< ram.burst <<" || Deadline -> Dias: " << entero<<" a las "<<resto<<"|| Hrs Totales : "<< ram.deadline << endl; 
        arc <<"Ramo n°:" << ram.id <<" || Burst: "<< ram.burst <<" || Deadline -> Dias: " << entero<<" a las "<<resto<<"|| Hrs Totales : "<< ram.deadline << endl; 
    }

    if(horas2 + ram.burst >= 24)
    {
        dias2 = dias2 + 1; 
        horas2 = horas2 + ram.burst - 24;

        if(dias2 >= entero)
        {
            dias2 = entero;
            horas2 = resto;
        }
    }
    else
    {
        horas2 = horas2 + ram.burst;
    }

    if(primero == 0)
    {
        cout << "Se supone un inicio Lunes 8:00 hrs"<< endl << endl; 
        arc << "Se supone un inicio Lunes 8:00 hrs\n \n" << endl;
        cout << "Dia :Lunes|| Hora :8:00 " << endl; ;
        arc <<"Dia :Lunes|| Hora :8:00 \n" << endl; 
        primero++;
    }

    cout << "Dia :" << diaSemana(&dias2) <<  "|| Hora : " << horas2<<":00" <<endl;
    arc << "Dia :" << diaSemana(&dias2) <<  "|| Hora : " << horas2<<":00" <<endl;

    

    for(i=0; i<(0xFFFFFFF);i++);
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(){

    int cont = 0 ,nRamo,err;
    arc.open("texto.txt",ios::out); 

    cout << "¿Cuantos ramos debe estudiar ?" << endl ;;
    cin >> nRamo; 
    pthread_t hilos[nRamo];
    int burst[nRamo], diaLim[nRamo], horaLim[nRamo], deadline[nRamo],j ; 
    cout << "Ingrese -> Horas Semanales | Dia Limite [0-6] | Hora limite [0-23]hrs" << endl ;
    cout << "Puede ingresar todo separado por un espacio." << endl << endl;

    for(j = 0; j < nRamo; j++){
        cout << "Asignatura: " << j+1 << endl; 
        cin >> burst[j] ;
        cin >> diaLim[j];
        cin >> horaLim[j];
        deadline[j] = transformacion(diaLim[j], horaLim[j]);
        Ramos.push(Ramo(burst[j], diaLim[j], horaLim[j], deadline[j], j+1));
        printf("-------------------------------------------------------------------\n");
    }
    cout << "Los valores son los siguientes:" << endl;
    for(int temp = 0;temp < j;temp++)
    {
        if(temp == 0)
        {
            cout << "|   N    |Est. Sem|Dia Lim | Hr Lim |" << endl;
            arc << "|   N    |Est. Sem|Dia Lim | Hr Lim |" << endl;
        }
        cout<<"|   "<<temp+1<<"    |   "<< burst[temp]<<"    |   "<<diaLim[temp]<<"    |   "<<horaLim[temp]<<"    |   "<< endl;
        arc<<"|   "<<temp+1<<"    |   "<< burst[temp]<<"    |   "<<diaLim[temp]<<"    |   "<<horaLim[temp]<<"    |   "<< endl;
    }
    cout <<"-------------------------------------------------------------------"<< endl;
    arc <<"-------------------------------------------------------------------"<< endl;
    while(cont < nRamo) 
    {
        err = pthread_create(&(hilos[cont]), NULL, &ThreadSpace, NULL);
        cont++;
    }
    for(int j = 0; j < nRamo; j++){
        pthread_join(hilos[j], NULL);
    }
    pthread_mutex_destroy(&lock);
	return 0;
    
}