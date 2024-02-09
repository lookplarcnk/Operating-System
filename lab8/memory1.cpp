#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#define NUM_P 6
#define NUM_LOGI 4

struct Logical
{
    int page_No;		// เก็บหมายเลขหน้าที่ต้องการอ้างอิง
    char offset[100];	// เก็บค่า offset หรือตำแหน่งภายในหน้านั้นๆ
};

struct ProcessPage
{
    int page_No;		// เก็บหมายเลขหน้าที่ต้องการอ้างอิง
    char offset[100];	// เก็บค่า offset หรือตำแหน่งภายในหน้านั้นๆ
};

char str[100] = {'\0'}; // แก้ str เป็น char array และกำหนดค่าเริ่มต้นเป็น '\0'
int memoryPage = 8;
int systemBus = 24;
struct ProcessPage PageTable[NUM_P + 1] = {{},
                                     {1, "0000010100101001"},
                                     {2, "0000010100100011"},
                                     {3, "0000000100100100"},
                                     {4, "0000110101101011"},
                                     {5, "0000011101101100"},
                                     {6, "0000111101100111"}};
struct Logical logi[NUM_LOGI + 1] = {{},
                               {1, "25"},
                               {2, "43"},
                               {3, "7"},
                               {5, "50"}};

void binTohex(char *str, char bin[])
{
    int dec = 0;
    char numchar;

    for (int i = 0; i < (strlen(bin)); i += 4)
    {
        for (int j = 0; j <= 3; j++)
        {
            dec += (bin[i + j] - '0') * (int)pow(2, 3 - j);
        }
        switch (dec)
        {
        case 15:
            strcat(str, "F");
            break;
        case 14:
            strcat(str, "E");
            break;
        case 13:
            strcat(str, "D");
            break;
        case 12:
            strcat(str, "C");
            break;
        case 11:
            strcat(str, "B");
            break;
        case 10:
            strcat(str, "A");
            break;
        default:
            numchar = dec + '0';
            strncat(str, &numchar, 1);
        }
        dec = 0;
    }
}

void hexTobin(char *str, char hex[])
{
    for (int i = 0; i < strlen(hex); ++i)
    {
        switch (hex[i])
        {
        case '0':
            strcat(str, "0000");
            break;
        case '1':
            strcat(str, "0001");
            break;
        case '2':
            strcat(str, "0010");
            break;
        case '3':
            strcat(str, "0011");
            break;
        case '4':
            strcat(str, "0100");
            break;
        case '5':
            strcat(str, "0101");
            break;
        case '6':
            strcat(str, "0110");
            break;
        case '7':
            strcat(str, "0111");
            break;
        case '8':
            strcat(str, "1000");
            break;
        case '9':
            strcat(str, "1001");
            break;
        case 'A':
            strcat(str, "1010");
            break;
        case 'B':
            strcat(str, "1011");
            break;
        case 'C':
            strcat(str, "1100");
            break;
        case 'D':
            strcat(str, "1101");
            break;
        case 'E':
            strcat(str, "1110");
            break;
        case 'F':
            strcat(str, "1111");
            break;
        }
    }
}

const char *Convert(char *star, char number[], const char *base) // แก้ base เป็น const char*
{
    strcpy(str, "");
    if (strcmp(base, "2") == 0)
    {
        hexTobin(str, number);
    }
    else if (strcmp(base, "16") == 0)
    {
        binTohex(str, number);
    }
    else
    {
        return "error!!!";
    }
    return str;
}

void show()
{
    printf("Logical Address to Physical Address Method Paging\n");
    printf("------------------------------------------------------------\n");
    printf("Logical Address\t\t");
    printf("Process page table\n");
    for (int i = 1; i < NUM_P + 1; ++i)
    {
        if (logi[i].page_No != 0)
            printf("%02d %3sH\t\t\t", logi[i].page_No, logi[i].offset);
        else
            printf("\t\t\t");
        printf("page: %d  %sB", PageTable[i].page_No, PageTable[i].offset);
        printf("\n");
    }
    printf("------------------------------------------------------------\n");
    printf("Memory per Page(256 Byte) = (%d^%d) = %2d bit n bitoffset\n", 2, memoryPage, memoryPage);
    printf("CPU Address bus                     = %d bit m bit all\n", systemBus);
    printf("bit for frame             = (%d-%d) = %2d bit m-n bit page num\n", systemBus, memoryPage, (systemBus - memoryPage));
    printf("------------------------------------------------------------\n");
}
void Physical()
{
    for (int i = 1; i < NUM_LOGI + 1; ++i)
    {
        for (int j = 1; j < NUM_P + 1; ++j)
        {
            if (logi[i].page_No == PageTable[j].page_No)
            {
                printf("Logical\t%sH\t\t%024s B\n", logi[i].offset, Convert(str, logi[i].offset, "2"));
                printf("\t%02d\t=\tPage No.%d\n", logi[i].page_No, logi[i].page_No);
                printf("\t%2s\t=\tPage off.%s\n", logi[i].offset, logi[i].offset);
                printf("------------------------------------------------------------\n");
                printf("Physical\t\tf = %s B\n", PageTable[j].offset);
                printf("\t\t\t%7sFrame No. | Page off.\n", "");
                printf("Physical Address\t%s | %s B\n", PageTable[j].offset, Convert(str, logi[i].offset, "2"));
                printf("\t\t\t%11s %s | %s H\n", "", Convert(str, PageTable[j].offset, "16"), logi[i].offset);
                printf("------------------------------------------------------------\n");
                break;
            }
        }
    }
}

int main()
{
    show();
    Physical();
    return 0;
}
