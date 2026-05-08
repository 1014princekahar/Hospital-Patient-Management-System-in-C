#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 200

const char doctor[] = "doctors.txt", patient[] = "appointment.txt", token_file[] = "token.txt";

FILE *fp, *fp1;

int f = -1, r = -1, top = -1, Token = 100;

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

Patient queue[MAX];
Patient stack[MAX];

void clearScreen();
void insertQueue(Patient *p);
void loadDoctors();
int doctorValidation(Patient *p);
void addPatient();
void servePatient();
void undoServe();

/* ******************************* Main Function( Switch Case ) ******************************* */

int main()
{
    int ch;
    clearScreen();

    // Load last token from token.txt
    fp = fopen(token_file, "r");
    if (fp != NULL)
    {
        fscanf(fp, "%d", &Token);
        fclose(fp);
    }

    do
    {

        printf(" ====> Hospital Patient Management System <=== \n");
        printf("1. Load Doctors\n");
        printf("2. Add Patient\n");
        printf("3. Serve Patient\n");
        printf("4. Display Waiting Patients\n");
        printf("5. Undo Last Served Patient\n");
        printf("6. Save Patient History\n");
        printf("7. Exit\n");

        printf("Enter Your Choice : ");
        scanf("%d", &ch);

        switch (ch)
        {
        case 1:
            loadDoctors();
            break;
        case 2:
            addPatient();
            break;
        case 3:
            servePatient();
            break;

        case 4:
            // addPatient();
            break;

        case 5:
            undoServe();
            break;

        case 6:
            // addPatient();
            break;

        case 7:
            // addPatient();
            break;
        default:
            printf("Invalid Choice...\n");
        }
    } while (ch != 7);
    return 0;
}

/* ******************************* Clear Screen ******************************* */

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/* ******************************* Insert Patient into Queue ******************************* */

void insertQueue(Patient *p)
{
    if (r == MAX - 1)
    {
        printf("Queue is Full...\n");
        return;
    }

    if (f == -1)
    {
        f = 0;
    }

    queue[++r] = *p;
}

/* ******************************* Load Doctors ******************************* */

void loadDoctors()
{
    clearScreen();

    printf(" ====> Load Doctors <==== \n");
    int doctor_number;

    printf("How Many Doctors You Want to Add : ");
    scanf("%d", &doctor_number);
    getchar(); // Clear newline

    Doctor dr;

    fp = fopen(doctor, "a");

    if (fp == NULL)
    {
        printf("Unable to Open %s\n", doctor);
        return;
    }

    for (int i = 0; i < doctor_number; i++)
    {
        printf("\nDoctor %d Details\n", i + 1);

        printf("Enter Doctor Name : ");
        scanf(" %[^\n]", dr.name);
        getchar(); // Clear newline

        fprintf(fp,
                "Doctor Name : %s\n",
                dr.name);
    }

    fclose(fp);

    printf("\nDoctors Added Successfully...\n");
}

/* ******************************* Check Doctor Validation ******************************* */

int doctorValidation(Patient *p)
{
    char line[MAX];
    char doctorName[MAX];

    fp1 = fopen(doctor, "r");

    if (fp1 == NULL)
    {
        printf("Doctor File Not Found...\n");
        return 0;
    }

    while (fgets(line, sizeof(line), fp1) != NULL)
    {
        /* Check Only Doctor Name Line */

        if (strncmp(line, "Doctor Name : ", 14) == 0)
        {
            /* Extract Doctor Name */

            strcpy(doctorName, line + 14);

            /* Remove Newline */

            doctorName[strcspn(doctorName, "\n")] = '\0';

            /* Compare Doctor Name */

            if (strcmp(p->doctor, doctorName) == 0)
            {
                fclose(fp1);
                return 1;
            }
        }
    }

    fclose(fp1);

    return 0;
}

/* ******************************* Add Patient ******************************* */

void addPatient()
{
    clearScreen();

    Patient p;
    int patient_number;

    printf("How Many Patients You Want to Add : ");
    scanf("%d", &patient_number);
    getchar(); // Clear newline from buffer

    fp = fopen(patient, "a");

    if (fp == NULL)
    {
        printf("Unable to Open %s\n", patient);
        return;
    }

    for (int i = 0; i < patient_number; i++)
    {
        printf("\n========== Patient %d Details ==========\n", i + 1);

        printf("Enter Patient First Name : ");
        scanf(" %[^\n]", p.name.firstName);

        printf("Enter Patient Last Name : ");
        scanf(" %[^\n]", p.name.lastName);

        printf("Enter Patient Age : ");
        scanf("%d", &p.age);
        getchar(); // Clear newline

        if (p.age <= 0)
        {
            printf("Invalid Age...\n");
            i--;
            continue;
        }

        printf("Enter Disease : ");
        scanf(" %[^\n]", p.disease);

        printf("Enter Doctor Name : ");
        scanf(" %[^\n]", p.doctor);

        if (strlen(p.doctor) == 0)
        {
            printf("Invalid Doctor Name...\n");
            i--;
            continue;
        }

        /* Doctor Validation */

        if (!doctorValidation(&p))
        {
            printf("Doctor Not Found...\n");
            i--;
            continue;
        }

        /* Dynamic Token Generation */

        p.token = ++Token;

        /* Insert Into Queue */

        insertQueue(&p);

        /* Display Token */

        printf("\n====================================\n");
        printf("Token Generated Successfully\n");
        printf("Token No : %d\n", p.token);
        printf("Patient  : %s %s\n",
               p.name.firstName,
               p.name.lastName);
        printf("Doctor   : %s\n", p.doctor);
        printf("====================================\n");

        /* Save Into File */

        fprintf(fp,
                "Token : %d\n"
                "First Name : %s\n"
                "Last Name : %s\n"
                "Age : %d\n"
                "Disease : %s\n"
                "Doctor : %s\n\n",
                p.token,
                p.name.firstName,
                p.name.lastName,
                p.age,
                p.disease,
                p.doctor);

        fflush(fp); // Flush buffer immediately
    }

    fclose(fp);

    // Save token to token.txt
    fp = fopen(token_file, "w");
    if (fp != NULL)
    {
        fprintf(fp, "%d", Token);
        fclose(fp);
    }

    printf("\nPatients Added Successfully...\n");
}

/* ******************************* Serve Patient ******************************* */

void servePatient()
{
    clearScreen();

    if (f == -1 || f > r)
    {
        printf("No Patients in Queue...\n");
        return;
    }

    Patient p = queue[f];
    char filename[MAX];

    // Create filename from doctor name
    strcpy(filename, p.doctor);
    strcat(filename, ".txt");

    // Open or create doctor-specific file
    fp = fopen(filename, "a");
    if (fp == NULL)
    {
        printf("Unable to create %s\n", filename);
        return;
    }

    // Write appointment details to doctor's file
    fprintf(fp,
            "\n========== Doctor: %s =========\n"
            "Token : %d\n"
            "Patient Name : %s %s\n"
            "Age : %d\n"
            "Disease : %s\n"
            "============================\n\n",
            p.doctor,
            p.token,
            p.name.firstName,
            p.name.lastName,
            p.age,
            p.disease);

    fclose(fp);

    // Push served patient onto stack for undo
    if (top < MAX - 1)
    {
        stack[++top] = p;
    }

    // Display patient details
    printf("\n====== Patient Served Successfully ======\n");
    printf("Token : %d\n", p.token);
    printf("Patient Name : %s %s\n", p.name.firstName, p.name.lastName);
    printf("Doctor : %s\n", p.doctor);
    printf("Appointment saved to %s\n", filename);
    printf("========================================\n");

    // Move to next patient
    f++;
}

/* ******************************* Undo Serve Patient ******************************* */

void undoServe()
{
    clearScreen();

    if (top == -1)
    {
        printf("No Served Patient to Undo...\n");
        return;
    }

    Patient p = stack[top--];

    // Push patient back to queue
    if (r < MAX - 1)
    {
        f--;
        queue[r + 1] = p;
        r++;
    }

    printf("\n====== Last Served Patient Restored ======\n");
    printf("Token : %d\n", p.token);
    printf("Patient Name : %s %s\n", p.name.firstName, p.name.lastName);
    printf("Doctor : %s\n", p.doctor);
    printf("========================================\n");
}
