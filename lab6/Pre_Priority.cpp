#include <stdio.h>
#define NP 5 // number Process

typedef struct{
    int indexP;
    int startP;
}GanttChart;
typedef struct{
    int indexP;
    int BurtT;
    int Priority;
}Queue;
typedef struct{
    int BurtT;
    int ArrivalT;
    int Priority;
}Process;
//Process  burt time , Arrival time , Priority
Process P[NP+1]  =  {{0},
        /*P1*/ {    9     ,       1      ,      3},     // P1 = P[1]
        /*P2*/ {    3     ,       1      ,      5},     // P2 = P[2]
        /*P3*/ {    5     ,       3      ,      1},     // P3 = P[3]
        /*P4*/ {    4     ,       4      ,      4},     // P4 = P[4]
        /*P5*/ {    2     ,       7      ,      2}};    // P5 = P[5]

GanttChart Gantt[30];
Queue  Q[30];
int NT = 0;     //Time
int NG = 0;     //number GanttChart
int SP = 0;     //ตัวชี้ค่าในคิว

void push(int indexP, int LeftTimeBurt){        //เก็บโปรเซสไว้ในคิว
    SP++;
    Q[SP].indexP = indexP;
    Q[SP].BurtT  = LeftTimeBurt;
    Q[SP].Priority  = P[indexP].Priority;
}
void pop(){                   //นำโปรเซสออกจากคิว
    if(SP == 0)
        printf("UNDER FLOW!!!\n");
    SP--;
}

Queue sortQ(){                //เรียงข้อมูลจากน้อยไปมาก
    Queue tempP;
    int j;
    for (int i = 1; i <SP ; ++i) {
        j = i+1;
        if(Q[i].Priority <= Q[j].Priority){
            tempP = Q[i];
            Q[i]  = Q[j];
            Q[j]  = tempP;
        }
    }
    return Q[SP];
}

int minPriority(int indexP, int tempP[], int *countP) {  //ในกรณีที่มีโปรเซสเกิดในเวลาเดียวกัน หาค่าโปรเซสที่มีลำดับความสำคัญสูงที่สุด
    int minPriority = P[indexP].Priority;
    int minPro = indexP;
    *countP = 0;
    for (int i = 1; i <= NP; ++i)
        if (P[indexP].ArrivalT == P[i].ArrivalT) {
            tempP[*countP] = i;                     //เก็บโปรเซสที่เกิดในเวลาเดียวกัน
            *countP+=1;                             //จำนวนโปรเซสที่เกิดในเวลาเดียวกัน
            if(P[i].Priority < minPriority ){
                minPriority = P[i].Priority;
                minPro = i;
            }
        }
    return minPro;                                  //โปรเซสที่มีลำดับความสำคัญสูงที่สุด
}

void pushSynchronous(int indexP, int tempP[], int countP) {//เก็บโปรเซสที่เกิดพร้อมกันไว้ในคิว(ในเวลาเดียวกัน)
    for (int i = 0; i < countP ; ++i)
        if(tempP[i] != indexP )                            //ยกเว้นโปรเซสที่มีลำดับความสำคัญสูงที่สุด
            push(tempP[i],P[tempP[i]].BurtT);
}

void getData(int indexP, int time_i){
    Gantt[NG].indexP = indexP;          //เก็บโปรเซสเพื่อทำ GanttChart
    Gantt[NG].startP = time_i;          //เก็บเวลาเริ่มทำงานโปรเซสเพื่อทำ GanttChart
    NG++;
}

void P_Priority(){
    int indexP = 0, ENDPro = 0, runningP = 0;
    int tempP[NP], countP = 0;
    for (int i = 0; i <= NT; ++i) {              //i แทนเวลา (Time)
        for (int j = 1; j <= NP; ++j) {
            if(i == P[j].ArrivalT){              //ณ เวลาที่ i มีโปรเซส 1 2 3 ...N เกิดขึ้นใหม่
                indexP = minPriority(j,tempP,&countP);
                if(i >= ENDPro){      //ณ เวลาที่ i ถ้าไม่มีโปรเซสลกำลังทำงาน(ไม่มีการใช้ทรัพยากร)
                    pushSynchronous(indexP,tempP,countP);//ถ้ามีโปรเซสเกิดขึ้นพร้อกัน(ในเวลาเดียวกัน)
                    ENDPro = i + P[indexP].BurtT;//เวลาที่โปรเซสจะจบการทำงาน
                    runningP = indexP;           //โปรเซสที่กำลังทำงาน
                    getData(indexP,i);           //เก็บข้อมูลโปรเซส
                }else{                           //ณ เวลาที่ i ถ้ามีโปรเซสลกำลังทำงาน(มีการใช้ทรัพยากร) หรือ มีโปรเซสออยู่ในคิว
                    if(P[indexP].Priority < (P[runningP].Priority) && i < ENDPro){//ให้เช็คว่าโปรเซสที่เกิดใหม่มีลำดับความสำคัญสูงกว่า โปรเซสที่กำลังทำงานใช่หรือไม่
                        pushSynchronous(indexP,tempP,countP);
                        push(runningP, (P[runningP].BurtT - i));  //เก็บโปรเซสที่กำลังทำงานไว้ในคิว และเวลาทำงานที่เหลือ
                        ENDPro = i + P[indexP].BurtT;
                        runningP = indexP;                      //โปรเซสเกิดใหม่ที่มีลำดับความสำคัญสูงกว่า เริ่มทำงาน
                        getData(indexP, i);
                    }else {
                        push(indexP, P[indexP].BurtT);//เก็บค่าโปรเซสที่เกิดใหม่ ณ เวลาที่ i ไว้ในคิว
                        pushSynchronous(indexP, tempP, countP);
                    }
                }
                break;
            }
        }
        if (i >= ENDPro && SP != 0) {            //ณ เวลาที่ i ถ้าไม่มีโปรเซสไหนทำงาน(ทรัพยากรว่าง) แต่ ยังมีโปรเซสเหลืออยู่ในคิว
            Queue indexPQ;
            indexPQ = sortQ();                    //เรียงโปรเซสในคิว โดยดูจากโปรเซสที่มีลำดับความสำคัญสูงที่สุด
            pop();                               //นำโปรเซสออกจากคิว
            ENDPro = i + indexPQ.BurtT;
            runningP = indexPQ.indexP;
            getData(indexPQ.indexP,i);
        }
    }
}

float waitProcess(int indexP){                              // คำนวณหา เวลารอของโปรเซสที่ i
    int count = 0;
    float waitT =0, end = 0;
    for (int i = 0; i <NG ; i++) {
        if(Gantt[i].indexP == indexP){
            if(count == 0){                                 // เวลาโปรเซสได้เข้าทำงาน - เวลาเเกิดของโปรเซส
                waitT += (float)Gantt[i].startP - P[indexP].ArrivalT;
                end    = (float)Gantt[i+1].startP;
                count++;
            }else{
                waitT += (float)Gantt[i].startP - end;      // เวล่ที่โปรเซสได้เข้าทำงานอีกครั้ง - เวลาที่โปรเซสจบการทำงานครั้งก่อน
                end    = (float)Gantt[i+1].startP;
            }
        }
    }
    return waitT;
}

void calNT(){                                               //คำนวณหาผลรวมของ burt time
    int sumBurt = 0;
    int minArrival = P[1].ArrivalT;
    for (int i = 1; i <= NP; ++i) {
        if(P[i].ArrivalT < minArrival){
            minArrival = P[i].ArrivalT;
        }
        sumBurt += P[i].BurtT;
    }
    NT = minArrival + sumBurt;                             //เวลาที่เริ่มเกิดโปรเซสตัวแรก + ผลรวมเวลที่ใช้ในการทำงานของโปรเซส
}

int main(){
    calNT();
    printf("# Miss Ratchaneekorn Chuadee ID:65543206077-1 Sec 02\n");
    printf("# OUTPUT LAB6 CPU Scheduling\n");
    printf("# Priority (SJF Preemptive) \n");
    printf("Sequence process is :");
    P_Priority();
    for (int i = 0; i <NG ; i++) {
        printf("P%d", Gantt[i].indexP);
        if(i<NG-1)
            printf("->");
    }
    printf("\n------------------------------------------------------------\n");
    printf("Wait time of process (millisecond)\n");
    for (int i = 1; i <= NP; i++) {
        printf("| P%-10d", i);
    }
    printf("\n");
    float sum = 0, avgTime;
    for (int i = 1; i <= NP; i++) {
        printf("| %-11.2f",waitProcess(i));
        sum += waitProcess(i);
    }
    avgTime = sum/NP;
    printf("\nAverage time is %.2f", avgTime);
    printf("\nTurnaround time\n");
    for (int i = 1; i <= NP; i++) {
        printf("|P%d = %-6.2fms ", i, waitProcess(i) + P[i].BurtT);
    }
    printf("\n------------------------------------------------------------\n");
    return 0;
}
