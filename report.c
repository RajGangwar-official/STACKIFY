#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50
#define NUM_SUBJECTS 5

// Global array of subject names
const char *SUBJECTS[NUM_SUBJECTS] = {"Mathematics", "Physics", "Chemistry", "English", "Computer Science"};

// Structure to hold student details
typedef struct {
    int rollNumber;
    char name[MAX_NAME_LEN];
    float marks[NUM_SUBJECTS];
    float totalMarks;
    float percentage;
    char grade;
} Student;

// Function Prototypes
void clearInputBuffer();
char calculateGrade(float percentage);
void addStudent(Student **students, int *count);
void displayReportCards(const Student *students, int count);
void saveToFile(const Student *students, int count, const char *filename);
void loadFromFile(Student **students, int *count, const char *filename);

int main() {
    Student *studentList = NULL;
    int studentCount = 0;
    int choice;
    const char *filename = "report_cards.dat";

    // Load existing records at startup
    loadFromFile(&studentList, &studentCount, filename);

    while (1) {
        printf("\n=========================================\n");
        printf("     SCHOOL REPORT CARD GENERATOR        \n");
        printf("=========================================\n");
        printf("1. Add New Student & Generate Report\n");
        printf("2. Display All Report Cards\n");
        printf("3. Save and Exit\n");
        printf("Enter your choice (1-3): ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                addStudent(&studentList, &studentCount);
                break;
            case 2:
                displayReportCards(studentList, studentCount);
                break;
            case 3:
                saveToFile(studentList, studentCount, filename);
                free(studentList); // Free dynamically allocated memory
                printf("Data saved successfully. Exiting program. Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice! Please select options 1, 2, or 3.\n");
        }
    }
}

// Utility function to clear standard input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Complex grade determination logic
char calculateGrade(float percentage) {
    if (percentage >= 90.0) return 'A';
    if (percentage >= 80.0) return 'B';
    if (percentage >= 70.0) return 'C';
    if (percentage >= 60.0) return 'D';
    if (percentage >= 40.0) return 'E';
    return 'F';
}

// Adds a student using dynamic array resizing (realloc)
void addStudent(Student **students, int *count) {
    // Allocate memory for one more student
    Student *temp = realloc(*students, (*count + 1) * sizeof(Student));
    if (temp == NULL) {
        printf("Memory allocation failed! Cannot add more students.\n");
        return;
    }
    *students = temp;

    Student *newStudent = &((*students)[*count]);

    // Input Roll Number
    printf("\nEnter Roll Number: ");
    while (scanf("%d", &newStudent->rollNumber) != 1 || newStudent->rollNumber <= 0) {
        printf("Invalid Roll Number! Enter a positive integer: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    // Input Name
    printf("Enter Student Name: ");
    fgets(newStudent->name, MAX_NAME_LEN, stdin);
    newStudent->name[strcspn(newStudent->name, "\n")] = 0; // Strip trailing newline

    // Input Marks with strict validation
    newStudent->totalMarks = 0;
    printf("\nEnter marks out of 100 for the following subjects:\n");
    for (int i = 0; i < NUM_SUBJECTS; i++) {
        while (1) {
            printf("%s: ", SUBJECTS[i]);
            if (scanf("%f", &newStudent->marks[i]) == 1 && newStudent->marks[i] >= 0.0 && newStudent->marks[i] <= 100.0) {
                newStudent->totalMarks += newStudent->marks[i];
                break;
            }
            printf("Invalid marks! Please enter a value between 0 and 100.\n");
            clearInputBuffer();
        }
    }

    // Calculations
    newStudent->percentage = newStudent->totalMarks / NUM_SUBJECTS;
    newStudent->grade = calculateGrade(newStudent->percentage);

    (*count)++;
    printf("\nReport card generated successfully for %s!\n", newStudent->name);
}

// Formats and prints a highly visual report card terminal view
void displayReportCards(const Student *students, int count) {
    if (count == 0) {
        printf("\nNo student records found. Add a student first!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        printf("\n--------------------------------------------------\n");
        printf("               ACADEMIC REPORT CARD               \n");
        printf("--------------------------------------------------\n");
        printf(" Roll No: %-10d | Name: %s\n", students[i].rollNumber, students[i].name);
        printf("--------------------------------------------------\n");
        printf(" Subject                 | Marks Obtained\n");
        printf("--------------------------------------------------\n");
        
        for (int j = 0; j < NUM_SUBJECTS; j++) {
            printf(" %-23s | %6.2f / 100\n", SUBJECTS[j], students[i].marks[j]);
        }
        
        printf("--------------------------------------------------\n");
        printf(" Total Marks: %6.2f / %d\n", students[i].totalMarks, NUM_SUBJECTS * 100);
        printf(" Percentage:  %6.2f%%\n", students[i].percentage);
        printf(" Final Grade:   [ %c ]\n", students[i].grade);
        printf(" Status:      %s\n", (students[i].grade == 'F') ? "FAIL" : "PASS");
        printf("--------------------------------------------------\n");
    }
}

// Binary file persistence (Save)
void saveToFile(const Student *students, int count, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }

    // Write the total record count first, then the array block
    fwrite(&count, sizeof(int), 1, file);
    if (count > 0) {
        fwrite(students, sizeof(Student), count, file);
    }
    fclose(file);
}

// Binary file persistence (Load)
void loadFromFile(Student **students, int *count, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        // File doesn't exist yet, which is fine for the first run
        *count = 0;
        return;
    }

    fread(count, sizeof(int), 1, file);
    if (*count > 0) {
        *students = malloc((*count) * sizeof(Student));
        if (*students == NULL) {
            printf("Memory allocation failed while loading data!\n");
            *count = 0;
            fclose(file);
            return;
        }
        fread(*students, sizeof(Student), *count, file);
    }
    fclose(file);
    printf("Successfully loaded %d student records from database.\n", *count);
}
