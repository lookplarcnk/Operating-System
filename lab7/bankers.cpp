#include <stdio.h>

int np,nr;     // number process and number resource
int Allocation[10][5],Max[10][5],Need[10][5],Available[10][5];  //
int resource[5],state[10],AvailDrives[5],AvailSUM[5];  //
int returnResources = 1;

void calAvailable(){ //คำรวณหา Available drives
    for (int i = 0; i < nr; ++i) {
        for (int j = 0; j < np; ++j) {
            AvailDrives[i] += Allocation[j][i];
        }
        AvailDrives[i] = resource[i] - AvailDrives[i];
        AvailSUM[i] = AvailDrives[i];
    }
}
void calNeed(){  //คำนวณหาทรัพยากรที่โปรเซสต้องการ
    for (int i = 0; i < np; ++i) { //ROW
        for (int j = 0; j < nr; ++j) {//COMLUMN
            Need[i][j] = Max[i][j] - Allocation[i][j];
        }
    }
}
void availSum(int i){  // คืนทรัพยากรณ์ที่ใช้แล้ว
    for (int j = 0; j < nr; ++j) {
        Available[i][j] = Allocation[i][j] + AvailSUM[j];
        AvailSUM[j] = Available[i][j];
    }
}
void Banker(){
    int Level = 0;
    for (int round = 1;returnResources == 1; ++round) {//จะวนรอบเรื่อยๆ ถ้ายังมีการคืนทรัยากรอยู่
        returnResources = 0;  //set ค่าเริ่มต้น
        for (int i = 0; i < np; ++i) {          // วนลูปตามจำนวนโปนเซส กำหนดให้เป็น ROW
            if(round == 1 || (round  >1 && state[i] == -1)) {
                //รอบที่ 1 จะตรวจสอบทุกโปรเซส แต่ถ้าเป็นรอบต่อไป จะตรวจสอบแค่โปรเซสที่มี state เป็น NULL
                for (int j = 0; j < nr; ++j) {  // วนลูปตามจำนวน resourceกำหนดให้เป็น COLUMN
                    Available[i][j] =-1;        // set ค่าเริ่มต้นให้ Available เป็น NULL
                    if (AvailSUM[j] >= Need[i][j]) {  //ถ้า Avali มีทรัพยากรเพียงพอต่อ ที่โปรเซสต้องการ (Need)
                        if (j == nr - 1) {      // Avali มีทรัพยากรจนครบเพียงพอจนครบ ตามจำนวน resource
                            availSum(i);        // คืนทรัพยากรณ์ที่ใช้แล้ว
                            state[i] = Level+=1; // ลำดับที่โปรเซสได้ใช้ทรัพยากร
                            returnResources = 1; // มีการคืนทรัพยากร
                        }
                    } else {                      //ถ้า Avali มีทรัพยากรไม่เพียงพอต่อ ที่โปรเซสต้องการ (Need)
                        state[i] = -1;            // state เป็น NULL
                        Available[i][j] =-1;      // Available เป็น NULL
                        break;
                    }
                }
            }
        }
    }
}

void input(){
    printf("->> Input state\n");
    printf("Enter number of process :");
    scanf("%d", &np);
    printf("Enter number of resource :");
    scanf("%d", &nr);
    printf("------------------------------------------------------\n");
    printf("Enter all unit of resource :\n");
    for (int i = 0; i < nr; ++i) {
        printf(" resource0%d : ", i + 1);
        scanf("%d", &resource[i]);
    }
    printf("\n------------------------------------------------------");
    for (int i = 0; i < np; ++i) {
        printf("\nEnter allocation of process : %d\n", i + 1);
        for (int j = 0; j < nr; ++j) {
            printf("resource0%d : ", i + 1);
            scanf("%d", &Allocation[i][j]);
        }
    }
    printf("------------------------------------------------------\n");
    for (int i = 0; i < np; ++i) {
        printf("\nEnter max of process : %d\n", i + 1);
        for (int j = 0; j < nr; ++j) {
            int temp;
            printf("resource0%d : ", i + 1);
            scanf("%d", &Max[i][j]);
        }
    }
    printf("------------------------------------------------------\n");
}

void inTable(int i, int temp[][5]) {
    for (int j = 0; j < nr; ++j) {
        if(temp[i][j] == -1) {
            printf("        ");
            break;
        }
        else
            printf("%2d ", temp[i][j]);
    }
}
void showTable(){ // ตาราง
    printf("\n===========================================================\n");
    printf("Poscess      Alloc      Max      Need       Avail      State\n         ");
    for (int i = 0; i < 4; ++i){
        printf("  ");
        for (int j = 0; j < nr; ++j)
            printf("%c  ", 65 + j);
    }
    printf("\n");
    for (int i = 0; i < np; ++i){
        printf("P%d\t  ",i+1);
        inTable(i,Allocation);printf("  ");
        inTable(i, Max);printf("  ");
        inTable(i, Need);printf("  ");
        inTable(i, Available);printf("    ");
        if(state[i] == -1)
            printf("NULL");
        else
            printf("%d", state[i]);
        printf("\n");
    }
    printf("===========================================================\n");
}
void result(){
    printf("Available drives = ");
    for (int i = 0; i < nr; ++i)
        printf("%d ", AvailDrives[i]);

    int First = 0;               // ใช้เก็บโปรเก็บโปรเซสที่ได้ใช้ทรัยพยากรอันดับแรก
    for (int i = 0; i < np; ++i)  // วนลูปหาโปรเซสแรกที่ได้เข้าทำงาน
        if(state[i] == 1) {
            First = i + 1;
            break;
        }
    if(First !=0){        //ถ้ามีโปรเซสได้ใช้ทรัพกรอันดับแรก ให้แสดง.....
        printf("\nFirst state allocate(");
        for (int i = 0; i < nr; ++i)
            printf("%d,", Need[First-1][i]);
        printf(") drives to ");
        printf("P%d\n", First);
        printf("P%d completes and releases(", First);
        for(int i = 0; i<nr; i++)
            printf("%d,",Available[First-1][i]);
        printf(")Available drives\n");
        printf("Next state ...\n");
    }

    int countNULL = 0;
    for (int i = 0; i < np; ++i)
        if(state[i] == -1)  // ถ้า state = NULL
            countNULL +=1;  // ให้นับจำนวน  NULL

    if(countNULL == np)    // ถ้า state เป็น NULL ทั้งหมด
        printf("\nIf grant last drive to any process may get deadlock = UNSAFE STATE\n**ALL process don't run to completion\n");
    else if(countNULL > 0 && countNULL < np) {  // ถ้า stateเป็น NULL แค่บางอัน
        printf("Not enough for process needs remaining(");
        for (int i = 0; i < np; ++i)
            if(state[i] == -1)
                printf("P%d,",i+1);
        printf(")\nProcess may get deadlock = UNSAFE STATE\n");
    }
    else {    // ถ้า stat ไม่มี NULL เลย
        printf("Sequences state are ");
        for (int i = 1; i <=np ; ++i)
            for (int j = 0; j <np ; ++j)
                if(i == state[j])
                    printf("P%d ",j+1);
        printf("\n**ALL process run to completion = SAFE STATE\n");
    }

}

int main(){
    input();   // input value
    calAvailable();
    calNeed();
    Banker();  // banker algorithm

    printf("%d process\n", np);
    printf("%d resource is ", nr);
    for (int i = 0; i < nr; ++i)
        printf("%c(%d instances) ", 65 + i, resource[i]);

    showTable();  //แสดงตาราง
    result();     //สรุปผลลัพธ์
}
