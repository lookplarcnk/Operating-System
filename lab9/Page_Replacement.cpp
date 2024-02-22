// input 2 1 2 3 7 6 2 3 4 2 1 5 6 3 2 3 6 2 1
#include <stdio.h>
#include <stdbool.h>

struct Frame
{
    int page;
    int count;      // ใช้สำหรับ FIFO
    int pastIndex;  // ใช้สำหรับ LRU
    int nextIndex;  // ใช้สำหรับ OPT
};

int page[19]; // อาร์เรย์เพื่อเก็บ 19 หน้า

// โปรโตไทป์ของฟังก์ชัน
bool isValueInArray(int val, struct Frame frame[], int size);
int max(struct Frame frame[], int size, int key);
int min(struct Frame frame[], int size, int key);
int searchNextIndex(int Nframe, int nowIndex);
int searchPastIndex(int Nframe, int nowIndex);
int FIFO();
int LRU();
int OPT();

int main()
{
    int key;
    printf("\n================ Page Replacement Program ===================\n");
    printf("Please input 19 pages : ");
    for (int i = 0; i < 19; i++)
    {
        scanf("%d", &page[i]);
    }
    printf("\nYour input : ");
    for (int i = 0; i < 19; i++)
    {
        printf("%d ", page[i]);
    }
    while (key != 4)
    {
        printf("\n\n================ Please select a menu =======================\n");
        printf("1. FIFO\n2. LRU\n3. OPT\n4. Exit Program\n");
        printf("Please input a key to continue : ");
        scanf("%d", &key);
        switch (key)
        {
        case 1:
            FIFO();
            break;
        case 2:
            LRU();
            break;
        case 3:
            OPT();
            break;
        }
    }
    return 0;
}

// ฟังก์ชันสำหรับการแทนที่หน้าโดยใช้วิธี FIFO
int FIFO()
{
    int n = 0;
    int pf = 0;
    printf("Please input the number of frames : ");
    scanf("%d", &n);
    struct Frame frame[n]; // อาร์เรย์ของเฟรม
    for (int e = 0; e < n; e++)
    {
        frame[e].page = -1;
        frame[e].count = 0;
    }
    for (int i = 0; i < 19; i++)
    {
        if (!isValueInArray(page[i], frame, n)) // ตรวจสอบค่าซ้ำ
        {
            if (isValueInArray(-1, frame, n)) // ตรวจสอบว่ามีเฟรมว่างหรือไม่
            {
                for (int e = 0; e < n; e++)
                {
                    if (frame[e].page == -1)
                    {
                        frame[e].page = page[i];
                        frame[e].count++;
                        pf++;
                        break;
                    }
                    else
                    {
                        frame[e].count++;
                    }
                }
            }
            else
            {
                for (int e = 0; e < n; e++)
                {
                    if (max(frame, n, 1) != e)
                    { // เพิ่ม count สำหรับหน้าที่ไม่ได้อ้างถึง
                        frame[e].count++;
                    }
                }
                frame[max(frame, n, 1)].page = page[i];
                frame[max(frame, n, 1)].count = 1;
                pf++;
            }
        }
        else
        { // กรณีหน้าอ้างถึงซ้ำ อัปเดต count สำหรับเฟรมทั้งหมด
            for (int e = 0; e < n; e++)
            {
                frame[e].count++;
            }
        }
        printf("\nInput page : %d\n", page[i]);
        printf("Frame Content :\n");
        for (int e = 0; e < n; e++)
        {
            if (frame[e].page == -1)
            {
                printf("Frame %d : [ ]\n", e + 1);
            }
            else
            {
                printf("Frame %d : [%d]\n", e + 1, frame[e].page);
            }
        }
        printf("=============================================================\n");
        printf("Page fault (FIFO) : %d\n", pf);
        printf("=============================================================\n");
    }
    return 0;
}

// ฟังก์ชันสำหรับการแทนที่หน้าโดยใช้วิธี LRU
int LRU()
{
    int n = 0;
    int pf = 0;
    printf("Please input the number of frames : ");
    scanf("%d", &n);
    struct Frame frame[n]; // อาร์เรย์ของเฟรม
    for (int e = 0; e < n; e++)
    {
        frame[e].page = -1;
        frame[e].pastIndex = 0;
    }
    for (int i = 0; i < 19; i++)
    {
        if (!isValueInArray(page[i], frame, n)) // ตรวจสอบค่าซ้ำ
        {
            if (isValueInArray(-1, frame, n)) // ตรวจสอบว่ามีเฟรมว่างหรือไม่
            {
                for (int e = 0; e < n; e++)
                {
                    if (frame[e].page == -1)
                    {
                        frame[e].page = page[i];
                        frame[e].pastIndex = i;
                        pf++;
                        break;
                    }
                    else
                    {
                        frame[e].pastIndex = searchPastIndex(frame[e].page, i);
                    }
                }
            }
            else
            {
                frame[min(frame, n, 2)].page = page[i];
                frame[min(frame, n, 2)].pastIndex = searchPastIndex(frame[min(frame, n, 2)].page, i);
                pf++;
            }
        }
        else
        { // กรณีหน้าอ้างถึงซ้ำ อัปเดต pastIndex สำหรับเฟรมทั้งหมด
            for (int e = 0; e < n; e++)
            {
                frame[e].pastIndex = searchPastIndex(frame[e].page, i);
            }
        }
        printf("\nInput page : %d\n", page[i]);
        printf("Frame Content :\n");
        for (int e = 0; e < n; e++)
        {
            if (frame[e].page == -1)
            {
                printf("Frame %d : [ ]\n", e + 1);
            }
            else
            {
                printf("Frame %d : [%d]\n", e + 1, frame[e].page);
            }
        }
        printf("=============================================================\n");
        printf("Page fault (LRU) : %d\n", pf);
        printf("=============================================================\n");
    }
    return 0;
}

// ฟังก์ชันสำหรับการแทนที่หน้าโดยใช้วิธี OPT
int OPT()
{
    int n = 0;
    int pf = 0;
    printf("Please input the number of frames : ");
    scanf("%d", &n);
    struct Frame frame[n]; // อาร์เรย์ของเฟรม
    for (int e = 0; e < n; e++)
    {
        frame[e].page = -1;
        frame[e].nextIndex = 0;
    }
    for (int i = 0; i < 19; i++)
    {
        if (!isValueInArray(page[i], frame, n)) // ตรวจสอบค่าซ้ำ
        {
            if (isValueInArray(-1, frame, n)) // ตรวจสอบว่ามีเฟรมว่างหรือไม่
            {
                for (int e = 0; e < n; e++)
                {
                    if (frame[e].page == -1)
                    {
                        frame[e].page = page[i];
                        frame[e].nextIndex = searchNextIndex(frame[e].page, i);
                        pf++;
                        break;
                    }
                    else
                    {
                        frame[e].nextIndex = searchNextIndex(frame[e].page, i);
                    }
                }
            }
            else
            {
                if (frame[min(frame, n, 1)].nextIndex == 0)
                {
                    frame[min(frame, n, 1)].page = page[i];
                    frame[min(frame, n, 1)].nextIndex = searchNextIndex(frame[min(frame, n, 1)].page, i);
                }
                else
                {
                    frame[max(frame, n, 2)].page = page[i];
                    frame[max(frame, n, 2)].nextIndex = searchNextIndex(frame[max(frame, n, 2)].page, i);
                }
                pf++;
            }
        }
        else
        { // กรณีหน้าอ้างถึงซ้ำ อัปเดต nextIndex สำหรับเฟรมทั้งหมด
            for (int e = 0; e < n; e++)
            {
                frame[e].nextIndex = searchNextIndex(frame[e].page, i);
            }
        }
        printf("\nInput page : %d\n", page[i]);
        printf("Frame Content :\n");
        for (int e = 0; e < n; e++)
        {
            if (frame[e].page == -1)
            {
                printf("Frame %d : [ ]\n", e + 1);
            }
            else
            {
                printf("Frame %d : [%d]\n", e + 1, frame[e].page);
            }
        }
        printf("=============================================================\n");
        printf("Page fault (OPT) : %d\n", pf);
        printf("=============================================================\n");
    }
    return 0;
}

// ฟังก์ชันสำหรับตรวจสอบว่าค่าอยู่ในอาร์เรย์ของเฟรมหรือไม่
bool isValueInArray(int val, struct Frame frame[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if (frame[i].page == val)
            return true;
    }
    return false;
}

// ฟังก์ชันสำหรับค้นหาดัชนีของเฟรมที่มีค่า count หรือ nextIndex สูงสุด
int max(struct Frame frame[], int size, int key)
{
    int index = 0;
    if (key == 1) // ใช้สำหรับ FIFO
    {
        int max = frame[index].count;
        for (int i = 0; i < size; i++)
        {
            if (frame[i].count > max)
            {
                max = frame[i].count;
                index = i;
            }
        }
    }
    else if (key == 2) // ใช้สำหรับ OPT
    {
        int max = frame[index].nextIndex;
        for (int i = 0; i < size; i++)
        {
            if (frame[i].nextIndex > max)
            {
                max = frame[i].nextIndex;
                index = i;
            }
        }
    }
    return index;
}

// ฟังก์ชันสำหรับค้นหาดัชนีของเฟรมที่มีค่า nextIndex หรือ pastIndex ต่ำสุด
int min(struct Frame frame[], int size, int key)
{
    int index = 0;
    if (key == 1) // ใช้สำหรับ OPT
    {
        int min = frame[index].nextIndex;
        for (int i = 0; i < size; i++)
        {
            if (frame[i].nextIndex < min)
            {
                min = frame[i].nextIndex;
                index = i;
            }
        }
    }
    else if (key == 2) // ใช้สำหรับ LRU
    {
        int min = frame[index].pastIndex;
        for (int i = 0; i < size; i++)
        {
            if (frame[i].pastIndex < min)
            {
                min = frame[i].pastIndex;
                index = i;
            }
        }
    }
    return index;
}

// ฟังก์ชันสำหรับค้นหาดัชนีถัดไปของหน้าในอาร์เรย์
int searchNextIndex(int Nframe, int nowIndex)
{
    int index = 0;
    for (int i = nowIndex + 1; i < 19; i++)
    {
        if (Nframe == page[i])
        {
            index = i; // หากเข้าถึงในอนาคต
            break;
        }
    }
    return index;
}

// ฟังก์ชันสำหรับค้นหาดัชนีที่ผ่านมาของหน้าในอาร์เรย์
int searchPastIndex(int Nframe, int nowIndex)
{
    int index = 0;
    for (int i = nowIndex; i >= 0; i--)
    {
        if (Nframe == page[i])
        {
            index = i; // ค้นหาการใช้งานในอดีตที่ใกล้ที่สุดในเฟรม
            break;
        }
    }
    return index;
}