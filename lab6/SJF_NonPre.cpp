#include <stdio.h>
#define NP 5            // number Process จำนวนกระบวนการ

typedef struct{
    int indexP;         // จำนวน process
    int startP;         // เริ่มกระบวนการ process
}GanttChart;            // เก็บลำดับการทำงานของกระบวนการ
typedef struct{
    int indexP;         // จำนวน process
    int BurtT;          // ระยะเวลาการทำงาน
}Queue;                 // เก็บกระบวนการที่มีเวลารอนานที่สุด
typedef struct{
    int BurtT;          // ระยะเวลาการทำงาน 
    int ArrivalT;       // ลำดับการเกิด
    int Priority;       // ลำดับความสำคัญ
}Process;               // เก็บข้อมูลเกี่ยวกับกระบวนการแต่ละกระบวนการ

     //Process  burt time , Arrival time , Priority
Process P[NP+1]  =  {{0},           // กำหนดให้ทุกกระบวนการมีค่า BurtT เป็น 0 และ ArrivalT เป็น 1
        /*P1*/ {    9     ,       1      ,      3},     // P1 = P[1]
        /*P2*/ {    3     ,       1      ,      5},     // P2 = P[2]
        /*P3*/ {    5     ,       3      ,      1},     // P3 = P[3]
        /*P4*/ {    4     ,       4      ,      4},     // P4 = P[4]
        /*P5*/ {    2     ,       7      ,      2}};    // P5 = P[5]

GanttChart Gantt[30];   // ตารางสำหรับเก็บลำดับการทำงานของกระบวนการ
Queue  Q[30];           // คิวสำหรับเก็บกระบวนการที่รอทำงาน
int NT = 0;             // Time เวลาปัจจุบัน
int NG = 0;             // number GanttChart จำนวนจุดในตาราง 
int SP = 0;             // ตัวชี้ค่าในคิว 

void push(int indexP){                  // เก็บโปรเซสไว้ในคิว
    SP++;                               // เพิ่มค่า SP ขึ้น 1
    Q[SP].indexP = indexP;              // เก็บหมายเลขของกระบวนการใน Q[SP].indexP
    Q[SP].BurtT  = P[indexP].BurtT;     // เก็บเวลาทำงานของกระบวนการใน Q[SP].BurtT
}
void pop(){                             // นำโปรเซสออกจากคิว
    if(SP == 0)                         // กำหนดตัวแปร SP = 0
        printf("UNDER FLOW!!!\n");      // แสดงว่าคิวว่างเปล่า แสดงข้อความแจ้งเตือน
    SP--;                               // ลดค่า SP ลง 1
}

Queue sortQ(){                          // เรียงข้อมูลจากน้อยไปมาก ใช้การวนลูปแบบบับเบิ้ลโซร์ต
    Queue tempP;                        // เก็บข้อมูลคิวระหว่างการเรียงลำดับ
    int j;                              // ตัวแปรวนซ้ำ (loop counter) ในการจัดเก็บค่าตำแหน่งของ element ในคิว
    for (int i = 1; i <SP ; ++i) {      // วนลูปจาก i = 1 ถึง SP - 1
        j = i+1;                        // กำหนด j = i + 1
        if(Q[i].BurtT <= Q[j].BurtT){   // เปรียบเทียบเวลาทำงานของกระบวนการที่ Q[i] กับ Q[j]
            tempP = Q[i];               // เก็บค่า element ในตำแหน่ง i ไว้ในตัวแปรชั่วคราว tempP
            Q[i]  = Q[j];               // นำค่า element ในตำแหน่ง j มาใส่ในตำแหน่ง i
            Q[j]  = tempP;              // นำค่าที่เก็บไว้ใน tempP (ซึ่งเป็น element เดิมที่ตำแหน่ง i) มาใส่ในตำแหน่ง j
        }
    }
    return Q[SP];                       // ส่งคืนค่า element สุดท้ายในคิวหลังจากการเรียงลำดับเสร็จสิ้น
}

int minBurt(int indexP, int tempP[], int *countP) {  // ในกรณีที่มีโปรเซสเกิดในเวลาเดียวกัน หาค่าโปรเซสที่ใช้เวลาทำงานน้อยที่สุด
    int minBurt = P[indexP].BurtT;                   // เก็บค่า BurtT ของโปรเซสที่มี indexP ที่กำหนดลงในตัวแปร minBurt
    int minPro = indexP;                             // เก็บค่าของตัวแปร indexP ลงในตัวแปร minPro
    *countP = 0;                                     // กำหนดค่าเริ่มต้นตำแหน่งของตัวแปร countP เท่ากับ 0 
    for (int i = 1; i <= NP; ++i)                    // วนซ้ำตั้งแต่ค่าเริ่มต้นของ i คือ 1 จนถึงค่าเป้าหมายของ i 
        if (P[indexP].ArrivalT == P[i].ArrivalT) {   // ตรวจสอบว่า ArrivalT ของ indexP เท่ากับเวลามาถึงของกระบวนการที่มี i หรือไม่
            tempP[*countP] = i;                     // เก็บโปรเซสที่เกิดในเวลาเดียวกัน
            *countP+=1;                             // จำนวนโปรเซสที่เกิดในเวลาเดียวกัน เพิ่มค่า *countP ขึ้น 1 
            if(P[i].BurtT < minBurt ){              // ตรวจสอบว่าเวลาทำงาน (BurtT) ของกระบวนการที่มีดัชนี i น้อยกว่าเวลาทำงานสั้นที่สุดของกระบวนการทั้งหมด (minBurt) หรือไม่
                minBurt = P[i].BurtT;               // ใส่ค่าเวลาทำงานของกระบวนการที่มีดัชนี i ให้กับตัวแปร minBurt
                minPro = i;                         // ใส่ค่าดัชนีของกระบวนการที่มีเวลาทำงานสั้นที่สุดให้กับตัวแปร minPro
            }
        }
    return minPro;                                  // โปรเซสที่ใช้เวลาทำงานน้อยที่สุด
}

void pushSynchronous(int indexP, int tempP[], int countP) { // เก็บโปรเซสที่เกิดพร้อมกันไว้ในคิว (ในเวลาเดียวกัน)
    for (int i = 0; i < countP ; ++i)                       // วนซ้ำตั้งแต่ค่าเริ่มต้นของ i คือ 0 จนถึงค่าเป้าหมายของ i คือ countP โดยเพิ่มค่า i ทีละ 1 ในแต่ละรอบ
        if(tempP[i] != indexP )                             // ยกเว้นโปรเซสที่ใช้เวลาทำงานน้อยที่สุด
            push(tempP[i]);

}
void getData(int indexP, int time_i){ // ส่งออกค่าเวลาทำงานของกระบวนการที่มีดัชนี indexP ณ เวลา time_i
    Gantt[NG].indexP = indexP;          // เก็บหมายเลขโปรเซสเพื่อทำ GanttChart
    Gantt[NG].startP = time_i;          // เก็บเวลาเริ่มทำงานโปรเซสเพื่อทำ GanttChart
    NG++;                               // เพิ่มค่า NG (จำนวน Gantt chart) ขึ้น 1
}

void SJF_NP(){                          // กำหนดลำดับการทำงานของกระบวนการ
    int indexP = 0, ENDPro = 0;
    int tempP[NP], countP = 0;
    for (int i = 0; i <= NT; ++i) {              // i แทนเวลา (Time)
        for (int j = 1; j <= NP; ++j) {
            if(i == P[j].ArrivalT){              // ณ เวลาที่ i มีโปรเซส 1 2 3 ...N เกิดขึ้นใหม่
                indexP = minBurt(j,tempP,&countP);// ถ้ามีโปรเซสเกิดในเวลาเดียวกันให้หาโปรเซสที่ใช้เวลาทำงานน้อยที่สุด
                if(i >= ENDPro && SP == 0){       // ณ เวลาที่ i ถ้าไม่มีโปรเซสลกำลังทำงาน(ไม่มีการใช้ทรัพยากร) และ ไม่มีโปรเซสไหนรออยู่ในคิว
                    pushSynchronous(indexP,tempP,countP); // เก็บโปรเซสที่เหลือไว้ในคิวถ้ามีโปรเซสมีกิดขึ้นพร้อกัน(ในเวลาเดียวกัน)
                    ENDPro = i + P[indexP].BurtT;
                    getData(indexP,i);           // เก็บข้อมูลโปรเซส
                }else{                           // ณ เวลาที่ i ถ้ามีโปรเซสลกำลังทำงาน(มีการใช้ทรัพยากร) หรือ มีโปรเซสออยู่ในคิว
                    push(indexP);                // เก็บค่าโปรเซสที่เกิดใหม่ ณ เวลาที่ i ไว้ในคิว
                    pushSynchronous(indexP,tempP,countP); // เก็บโปรเซสที่เหลือไว้ในคิวถ้ามีโปรเซสมีกิดขึ้นพร้อกัน(ในเวลาเดียวกัน)
                }
                break;
            }
        }
        if (i >= ENDPro && SP != 0) {            // ณ เวลาที่ i ถ้าไม่มีโปรเซสไหนทำงาน(ทรัพยากรว่าง) แต่ ยังมีโปรเซสเหลืออยู่ในคิว
            Queue indexPQ;
            indexPQ = sortQ();                    // เรียงโปรเซสในคิว โดยดูจากโปรเซสที่ใช้เวลาการทำงานน้อยที่สุด
            pop();                               // นำโปรเซสออกจากคิว
            ENDPro = i + indexPQ.BurtT;
            getData(indexPQ.indexP,i);
        }
    }
}

float waitProcess(int indexP){                              // คำนวณหา เวลารอของโปรเซสที่ i
    int count = 0;
    float waitT = 0, end = 0;
    for (int i = 0; i <NG ; i++) {
        if(Gantt[i].indexP == indexP){
            if(count == 0){                                 // เวลาโปรเซสได้เข้าทำงาน - เวลาเเกิดของโปรเซส
                waitT += (float)Gantt[i].startP - P[indexP].ArrivalT;
                end    = (float)Gantt[i+1].startP;
                count++;
            }else{
                waitT += (float)Gantt[i].startP - end;      // เวลาที่โปรเซสได้เข้าทำงานอีกครั้ง - เวลาที่โปรเซสจบการทำงานครั้งก่อน
                end    = (float)Gantt[i+1].startP;
            }
        }
    }
    return waitT;
}

void calNT(){                                               // คำนวณหาผลรวมของ burt time
    int sumBurt = 0;                                        // กำหนดค่าเริ่มต้น = 0
    int minArrival = P[1].ArrivalT;                         // เก็บค่าเวลามาถึงที่น้อยที่สุดเท่าที่พบจนถึงตอนนี้
    for (int i = 1; i <= NP; ++i) {                         // ตรวจสอบเงื่อนไขว่าค่า ArrivalT ของโปรเซสตัวที่ i น้อยกว่าค่า minArrival หรือไม่                         
        if(P[i].ArrivalT < minArrival){                     // ถ้าเงื่อนไขเป็นจริง จะเข้าสู่ภายในบล็อก if เพื่ออัพเดตค่า minArrival
            minArrival = P[i].ArrivalT;
        }
        sumBurt += P[i].BurtT;                              // คำนวณค่าผลรวมเวลาการทำงานทั้งหมดของโปรเซส
    }
    NT = minArrival + sumBurt;                             // เวลาที่เริ่มเกิดโปรเซสตัวแรก + ผลรวมเวลที่ใช้ในการทำงานของโปรเซส
}

int main(){
    calNT();    // เรียกใช้ฟังก์ชัน calNT()
    printf("# Miss Ratchaneekorn Chuadee ID:65543206077-1 Sec 02\n");
    printf("# OUTPUT LAB6 CPU Scheduling\n");
    printf("# SJF Non Preemptive \n");
    printf("Sequence process is :");
    SJF_NP();   // เรียกใช้ฟังก์ชัน SJF_NP()
    for (int i = 0; i <NG ; i++) {
        printf("P%d", Gantt[i]);
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
