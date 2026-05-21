#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 200

// Constants names for file 
const char doctorFile[] = "doctors.txt";
const char appointedDoctorFile[] = "appointedDoctors.txt";
const char appointmentFile[] = "appointment.txt";
const char servedFile[] = "servePatient.txt";
const char tokenFile[] = "token.txt";

FILE *fp, *fp1;

int f = -1, r = -1, top = -1, Token = 100;

/* ====================== Structures ====================== */

typedef struct
{
    char name[MAX];
} Doctor;

typedef struct
{
    char firstName[MAX];
    char lastName[MAX];
} Name;

typedef struct
{
    int token;
    Name name;
    int age;
    char disease[MAX];
    char doctor[MAX];
} Patient;

/* ====================== Function Prototypes ====================== */

void clearScreen();
void addDoctors();


/* ====================== Main Function ====================== */

int main()
{
    int ch;

    fp = fopen(tokenFile, "r");

    if (fp != NULL)
    {
        fscanf(fp, "%d", &Token);
        fclose(fp);
    }

    do
    {
        clearScreen();

        printf("\n");
        printf("=====================================================\n");
        printf("      Hospital Patient Management System\n");
        printf("=====================================================\n");

        printf("1. Add Doctors\n");
        printf("2. Show Doctors\n");
        printf("3. Add Patient\n");
        printf("4. Serve Patient\n");
        printf("5. Display Waiting Patients\n");
        printf("6. Undo Last Served Patient\n");
        printf("7. Exit\n");

        printf("=====================================================\n");

        printf("Enter Your Choice : ");
        scanf("%d", &ch);

        switch (ch)
        {
        case 1:
            addDoctors();
            break;

        case 2:
            // showDoctors();
            break;

        case 3:
            // addPatient();
            break;

        case 4:
            // servePatient();
            break;

        case 5:
            // displayPatients();
            break;

        case 6:
            // undoServe();
            break;

        case 7:
            // saveToken();
            printf("\nProgram Closed Successfully...\n");
            break;

        default:
            printf("\nInvalid Choice...\n");
        }

        if (ch != 7)
        {
            printf("\nPress Enter to Continue...");
            getchar();
            getchar();
        }

    } while (ch != 7);

    return 0;
}

/* ====================== Clear Screen ====================== */

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/* ====================== Add Doctors ====================== */

void addDoctors()
{
    clearScreen();

    int n;
    Doctor dr;

    printf("\n========== Add Doctors ==========\n");

    printf("How Many Doctors You Want to Add : ");
    scanf("%d", &n);

    fp = fopen(doctorFile, "a");

    if (fp == NULL)
    {
        printf("\nUnable to Open %s\n", doctorFile);
        return;
    }

    for (int i = 0; i < n; i++)
    {
        printf("\nDoctor %d\n", i + 1);

        printf("Enter Doctor Name : ");
        scanf(" %[^\n]", dr.name);

        fprintf(fp, "Doctor Name : %s\n", dr.name);
    }

    fclose(fp);

    printf("\nDoctors Added Successfully...\n");
}

