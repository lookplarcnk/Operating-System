#include <stdio.h>

struct Process
{
    int index ;
    int burstTime;
    int arrivalTime;
    int priority;
    int startTime;
    int endTime;
    int waitingTime;
    int turnaroundTime;
    int processed;
};

int stackmember[5] ={ 0,0,0,0,0} ;
int Q[50] ;
int op = 1 ;
int QuantumT = 5 ;



int mostarrival( struct Process processes[] ){
    int mostarrivalT = processes[0].arrivalTime ;
    for ( int i = 0 ; i < 5 ; i++ ){
        if ( processes[i].arrivalTime > mostarrivalT ){
            mostarrivalT = processes[i].arrivalTime ;
        }
    }
    return  mostarrivalT ;
}
int timeallprocesss( struct Process processes[] ){
    int mostarrivalT = mostarrival(processes) ;
    int sum ;
    for ( int i = 0 ; i < 5 ; i++ ){
        sum += processes[i].burstTime ;
    }
    return  mostarrivalT + mostarrivalT ;
}

float sumwait(struct Process processes[]){
    int sum = 0 ;
    for ( int i = 0 ; i < 5 ; i++ ){
        sum = sum + processes[i].waitingTime ;
    }
    return sum ;
}


void printing_result(struct Process processes[], int n)
{
    int alltime = timeallprocesss(processes) ;
    int currentTime = 0;
    int indexP ;
    int endP = 0 ;
    for (int i = 0; i < 5; i++)
    {
        processes[i].processed = processes[i].burstTime ;
    }
    
    printf("Sequence process is: ");
    for (int i = 0; i < alltime ; i++)
    {   for (int j = 0; j < 5 ; j++ )
        {   
        if ( processes[j].arrivalTime >= i && processes[j].processed > 0)
            {
                if ( processes[j].processed > QuantumT ){
                    processes[j].processed -= QuantumT ;
                    stackmember[op] += QuantumT ; 
                }
                else{
                    stackmember[op] += processes[j].processed ; 
                    processes[j].processed = 0 ;       
                } 
                Q[op] = j ;
                op++ ; 
                
            }
        }
    }
    for ( int i = 1; i < op ; i++)
    {
        indexP = Q[i] ;
        printf("P%d", processes[indexP].index);
        if (i < op-1 )
        {
            printf("->");
        }
    }

    for ( int i = 0; i < 5 ; i++){
        int sumtime = processes[ Q[0] ].arrivalTime ;
        int holderA = processes[i].arrivalTime ;
        for (int j = 0; j < op ; j++)
        {
            if( processes[i].index == processes[ Q[j] ].index ){
                processes[i].waitingTime += ( sumtime - holderA  ) ;
                holderA = sumtime + stackmember[j] ;
            }
            sumtime += stackmember[j] ;
        }
    }
    printf("\n------------------------------------------------------------\n");
    printf("Wait time of process (millisecond)\n");
    printf("|");
    for (int i = 0; i < n; i++)
    {
        printf(" P%d\t|  ", processes[i].index);
    }
    printf("\n|");
    for (int i = 0; i < n; i++)
    {
        // Ensure waiting time is non-negative
        printf(" %.2f\t|", (float)processes[i].waitingTime);
        processes[i].turnaroundTime = processes[i].burstTime + processes[i].waitingTime;
    }
    printf("\nAverage time is %.2f\n", sumwait(processes) / 5 );

    printf("Turnaround time\n");
    for (int i = 0; i < n; i++)
    {
        printf("|P%d = %d ms ", processes[i].index, processes[i].turnaroundTime);
    }
    printf("\n------------------------------------------------------------\n");
}

int main()
{
    printf("# Miss Ratchaneekorn Chuadee ID:65543206077-1 Sec 02 \n");
    printf("# OUTPUT LAB6 CPU Scheduling \n");
    printf("# Round Robin scheduling \n");
    printf("\n------------------------------------------------------------\n");
    struct Process processes[] = {
        {1, 9, 1, 3},
        {2, 3, 1, 5},
        {3, 5, 3, 1},
        {4, 4, 4, 4},
        {5, 2, 7, 2},
    };

    int n = sizeof(processes) / sizeof(processes[0]);

    printing_result(processes, n);

    return 0;
}