#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 200

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

/* ====================== Arrays ====================== */

Patient queue[MAX];
Patient stack[MAX];

/* ====================== Function Prototypes ====================== */

void clearScreen();

void addDoctors();
void showDoctors();

int doctorValidation(Patient *p);

void insertQueue(Patient *p);
Patient deleteQueue();

void pushStack(Patient p);
Patient popStack();

void addPatient();
void servePatient();
void displayPatients();
void undoServe();

void saveToken();
void saveAppointments();

void rewriteServedFile();
// Move a doctor from doctors.txt into appointedDoctors.txt when a patient is assigned
int moveDoctorToAppointed(char doctorName[]);
// Restore the doctor back to doctors.txt when the assigned patient is served
int restoreDoctorToDoctors(char doctorName[]);

void sortQueue();
int tokenExists(int token);

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
            showDoctors();
            break;

        case 3:
            addPatient();
            break;

        case 4:
            servePatient();
            break;

        case 5:
            displayPatients();
            break;

        case 6:
            undoServe();
            break;

        case 7:
            saveToken();
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

/* ====================== Show Doctors ====================== */

void showDoctors()
{
    clearScreen();

    char line[MAX];

    fp = fopen(doctorFile, "r");

    if (fp == NULL)
    {
        printf("\nDoctor File Not Found...\n");
        return;
    }

    printf("\n========== Doctors List ==========\n\n");

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        printf("%s", line);
    }

    fclose(fp);
}

/* ====================== Doctor Validation ====================== */

int doctorValidation(Patient *p)
{
    char line[MAX];
    char doctorName[MAX];

    // Check whether the requested doctor exists in doctors.txt
    fp1 = fopen(doctorFile, "r");

    if (fp1 == NULL)
    {
        printf("\nDoctor File Not Found...\n");
        return 0;
    }

    while (fgets(line, sizeof(line), fp1) != NULL)
    {
        if (strncmp(line, "Doctor Name : ", 14) == 0)
        {
            strcpy(doctorName, line + 14);

            doctorName[strcspn(doctorName, "\n")] = '\0';

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

/* ====================== Insert Queue ====================== */

void insertQueue(Patient *p)
{
    if (r == MAX - 1)
    {
        printf("\nQueue Overflow...\n");
        return;
    }

    if (f == -1)
    {
        f = 0;
    }

    queue[++r] = *p;
}

/* ====================== Delete Queue ====================== */

Patient deleteQueue()
{
    Patient p;

    if (f == -1 || f > r)
    {
        p.token = -1;
        return p;
    }

    p = queue[f];

    f++;

    return p;
}

/* ====================== Push Stack ====================== */

void pushStack(Patient p)
{
    if (top == MAX - 1)
    {
        printf("\nStack Overflow...\n");
        return;
    }

    stack[++top] = p;
}

/* ====================== Pop Stack ====================== */

Patient popStack()
{
    Patient p;

    if (top == -1)
    {
        p.token = -1;
        return p;
    }

    p = stack[top--];

    return p;
}

/* ====================== Duplicate Token Check ====================== */

int tokenExists(int token)
{
    if (f == -1 || f > r)
    {
        return 0;
    }

    for (int i = f; i <= r; i++)
    {
        if (queue[i].token == token)
        {
            return 1;
        }
    }

    return 0;
}

/* ====================== Sort Queue ====================== */

void sortQueue()
{
    if (f == -1 || f > r)
    {
        return;
    }

    for (int i = f; i <= r; i++)
    {
        for (int j = i + 1; j <= r; j++)
        {
            if (queue[i].token > queue[j].token)
            {
                Patient temp = queue[i];
                queue[i] = queue[j];
                queue[j] = temp;
            }
        }
    }
}

/* ====================== Save Appointments ====================== */

void saveAppointments()
{
    fp = fopen(appointmentFile, "w");

    if (fp == NULL)
    {
        printf("Unable to Open %s\n", appointmentFile);
        return;
    }

    if (f != -1 && f <= r)
    {
        for (int i = f; i <= r; i++)
        {
            fprintf(fp,
                    "====================================\n"
                    "Token : %d\n"
                    "Patient : %s %s\n"
                    "Age : %d\n"
                    "Disease : %s\n"
                    "Doctor : %s\n"
                    "====================================\n\n",
                    queue[i].token,
                    queue[i].name.firstName,
                    queue[i].name.lastName,
                    queue[i].age,
                    queue[i].disease,
                    queue[i].doctor);
        }
    }

    fclose(fp);
}

/* ====================== Rewrite Served File ====================== */

void rewriteServedFile()
{
    fp = fopen(servedFile, "w");

    if (fp == NULL)
    {
        return;
    }

    for (int i = 0; i <= top; i++)
    {
        fprintf(fp,
                "====================================\n"
                "Token : %d\n"
                "Patient : %s %s\n"
                "Age : %d\n"
                "Disease : %s\n"
                "Doctor : %s\n"
                "====================================\n\n",
                stack[i].token,
                stack[i].name.firstName,
                stack[i].name.lastName,
                stack[i].age,
                stack[i].disease,
                stack[i].doctor);
    }

    fclose(fp);
}

int moveDoctorToAppointed(char doctorName[])
{
    FILE *fp_doc = fopen(doctorFile, "r");
    FILE *fp_temp = fopen("temp_doctors.txt", "w");
    FILE *fp_app = fopen(appointedDoctorFile, "a");
    char line[MAX];
    char extractedDoctor[MAX];
    int moved = 0;

    if (fp_doc == NULL || fp_temp == NULL || fp_app == NULL)
    {
        if (fp_doc)
            fclose(fp_doc);
        if (fp_temp)
            fclose(fp_temp);
        if (fp_app)
            fclose(fp_app);
        return 0;
    }

    while (fgets(line, sizeof(line), fp_doc) != NULL)
    {
        if (strncmp(line, "Doctor Name : ", 14) == 0)
        {
            strcpy(extractedDoctor, line + 14);
            extractedDoctor[strcspn(extractedDoctor, "\n")] = '\0';

            if (strcmp(extractedDoctor, doctorName) == 0)
            {
                fprintf(fp_app, "Doctor Name : %s\n", doctorName);
                moved = 1;
                continue;
            }
        }

        fprintf(fp_temp, "%s", line);
    }

    fclose(fp_doc);
    fclose(fp_temp);
    fclose(fp_app);

    if (!moved)
    {
        remove("temp_doctors.txt");
        return 0;
    }

    remove(doctorFile);
    rename("temp_doctors.txt", doctorFile);
    return 1;
}

int restoreDoctorToDoctors(char doctorName[])
{
    FILE *fp_app = fopen(appointedDoctorFile, "r");
    FILE *fp_temp = fopen("temp_appointed.txt", "w");
    FILE *fp_doc = fopen(doctorFile, "a");
    char line[MAX];
    char extractedDoctor[MAX];
    int restored = 0;

    if (fp_app == NULL || fp_temp == NULL || fp_doc == NULL)
    {
        if (fp_app)
            fclose(fp_app);
        if (fp_temp)
            fclose(fp_temp);
        if (fp_doc)
            fclose(fp_doc);
        return 0;
    }

    while (fgets(line, sizeof(line), fp_app) != NULL)
    {
        if (strncmp(line, "Doctor Name : ", 14) == 0)
        {
            strcpy(extractedDoctor, line + 14);
            extractedDoctor[strcspn(extractedDoctor, "\n")] = '\0';

            if (strcmp(extractedDoctor, doctorName) == 0)
            {
                fprintf(fp_doc, "Doctor Name : %s\n", doctorName);
                restored = 1;
                continue;
            }
        }

        fprintf(fp_temp, "%s", line);
    }

    fclose(fp_app);
    fclose(fp_temp);
    fclose(fp_doc);

    if (!restored)
    {
        remove("temp_appointed.txt");
        return 0;
    }

    remove(appointedDoctorFile);
    rename("temp_appointed.txt", appointedDoctorFile);
    return 1;
}

/* ====================== Add Patient ====================== */

void addPatient()
{
    clearScreen();

    Patient p;

    int n;

    printf("\n========== Add Patient ==========\n");

    printf("How Many Patients You Want to Add : ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++)
    {
        printf("\n========== Patient %d ==========\n", i + 1);

        printf("Enter First Name : ");
        scanf(" %[^\n]", p.name.firstName);

        printf("Enter Last Name : ");
        scanf(" %[^\n]", p.name.lastName);

        printf("Enter Age : ");
        scanf("%d", &p.age);

        if (p.age <= 0)
        {
            printf("\nInvalid Age...\n");
            i--;
            continue;
        }

        printf("Enter Disease : ");
        scanf(" %[^\n]", p.disease);

        printf("Enter Doctor Name : ");
        scanf(" %[^\n]", p.doctor);

        if (!doctorValidation(&p))
        {
            printf("\nDoctor Not Found...\n");
            i--;
            continue;
        }

        // Doctor is assigned to this patient, so move doctor record to appointedDoctors.txt
        if (!moveDoctorToAppointed(p.doctor))
        {
            printf("\nUnable to appoint doctor.\n");
            i--;
            continue;
        }

        // Generate a unique token for the patient
        do
        {
            p.token = ++Token;
        } while (tokenExists(p.token));

        insertQueue(&p);

        sortQueue();

        saveAppointments();

        printf("\n====================================\n");
        printf("Token Generated Successfully\n");
        printf("Token No : %d\n", p.token);
        printf("Patient  : %s %s\n",
               p.name.firstName,
               p.name.lastName);
        printf("Doctor   : %s\n", p.doctor);
        printf("====================================\n");
    }

    saveToken();

    printf("\nPatients Added Successfully...\n");
}

/* ====================== Serve Patient ====================== */

void servePatient()
{
    clearScreen();

    Patient p;

    p = deleteQueue();

    if (p.token == -1)
    {
        printf("\nNo Patients in Queue...\n");
        return;
    }

    // Patient has been served, so restore the assigned doctor back into doctors.txt
    if (!restoreDoctorToDoctors(p.doctor))
    {
        printf("\nWarning: Unable to restore doctor to %s\n", doctorFile);
    }

    // Keep served patients in undo stack before writing served history
    pushStack(p);

    fp = fopen(servedFile, "a");

    if (fp != NULL)
    {
        fprintf(fp,
                "====================================\n"
                "Token : %d\n"
                "Patient : %s %s\n"
                "Age : %d\n"
                "Disease : %s\n"
                "Doctor : %s\n"
                "====================================\n\n",
                p.token,
                p.name.firstName,
                p.name.lastName,
                p.age,
                p.disease,
                p.doctor);

        fclose(fp);
    }

    saveAppointments();

    printf("\n========== Serving Patient ==========\n");

    printf("Token   : %d\n", p.token);

    printf("Patient : %s %s\n",
           p.name.firstName,
           p.name.lastName);

    printf("Disease : %s\n", p.disease);
    printf("Doctor  : %s\n", p.doctor);

    printf("=====================================\n");
}

/* ====================== Display Waiting Patients ====================== */

void displayPatients()
{
    clearScreen();

    if (f == -1 || f > r)
    {
        printf("\nNo Waiting Patients...\n");
        return;
    }

    printf("\n================ Waiting Patients ================\n\n");

    printf("Token\tPatient Name\t\tDoctor\n");

    printf("==================================================\n");

    for (int i = f; i <= r; i++)
    {
        printf("%d\t%s %s\t\t%s\n",
               queue[i].token,
               queue[i].name.firstName,
               queue[i].name.lastName,
               queue[i].doctor);
    }
}

/* ====================== Undo Serve ====================== */

void undoServe()
{
    clearScreen();

    Patient p;

    p = popStack();

    if (p.token == -1)
    {
        printf("\nNo Served Patient to Undo...\n");
        return;
    }

    if (f > 0)
    {
        queue[--f] = p;
    }
    else
    {
        if (f == -1)
        {
            f = 0;
            r = 0;
        }
        else
        {
            for (int i = r; i >= f; i--)
            {
                queue[i + 1] = queue[i];
            }

            r++;
        }

        queue[f] = p;
    }

    sortQueue();

    saveAppointments();

    rewriteServedFile();

    printf("\n====== Last Served Patient Restored ======\n");

    printf("Token : %d\n", p.token);

    printf("Patient : %s %s\n",
           p.name.firstName,
           p.name.lastName);

    printf("Doctor : %s\n", p.doctor);

    printf("==========================================\n");
}

/* ====================== Save Token ====================== */

void saveToken()
{
    fp = fopen(tokenFile, "w");

    if (fp != NULL)
    {
        fprintf(fp, "%d", Token);
        fclose(fp);
    }
}
