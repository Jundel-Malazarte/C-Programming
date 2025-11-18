/* 

STUDENT ACCOUNTING SYSTEM
This program allows users to manage student records, including inputting student information,
validating data, calculating fees based on course and subjects, and displaying detailed accounting information.

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_STUDENTS      100
#define MAX_NAME_LEN      50
#define MAX_COURSE_LEN    20
#define MAX_COURSE_DESC   100

#define UNITS_PER_SUBJECT 3
#define MIN_YEAR_LEVEL    1
#define MAX_YEAR_LEVEL    5
#define MIN_SUBJECTS      1
#define MAX_SUBJECTS      8

#define PRELIM_PCT    0.53f
#define MIDTERM_PCT   0.64f
#define SEMIFINAL_PCT 0.75f

/* COURSE CODE and related constants (no literal magic numbers/strings) */
#define COURSE_CODE_1     "Code1"
#define COURSE_DESC_1     "Course1"
#define COURSE_TUITION_1  356.75f
#define COURSE_REG_1      545.00f
#define COURSE_MISC_1     1000.45f
#define COURSE_LAB_1      1900.75f

#define COURSE_CODE_2     "Code2"
#define COURSE_DESC_2     "Course2"
#define COURSE_TUITION_2  357.75f
#define COURSE_REG_2      550.00f
#define COURSE_MISC_2     1050.35f
#define COURSE_LAB_2      1920.20f

#define COURSE_CODE_3     "Code3"
#define COURSE_DESC_3     "Course3"
#define COURSE_TUITION_3  345.94f
#define COURSE_REG_3      555.00f
#define COURSE_MISC_3     1100.25f
#define COURSE_LAB_3      1939.65f

#define COURSE_CODE_4     "Code4"
#define COURSE_DESC_4     "Course4"
#define COURSE_TUITION_4  312.62f
#define COURSE_REG_4      560.00f
#define COURSE_MISC_4     1150.15f
#define COURSE_LAB_4      1959.10f

#define COURSE_CODE_5     "Code5"
#define COURSE_DESC_5     "Course5"
#define COURSE_TUITION_5  378.44f
#define COURSE_REG_5      565.00f
#define COURSE_MISC_5     1200.05f
#define COURSE_LAB_5      1978.55f

#define COURSE_CODE_6     "Code6"
#define COURSE_DESC_6     "Course6"
#define COURSE_TUITION_6  326.11f
#define COURSE_REG_6      570.00f
#define COURSE_MISC_6     1249.95f
#define COURSE_LAB_6      1998.00f

#define COURSE_CODE_7     "Code7"
#define COURSE_DESC_7     "Course7"
#define COURSE_TUITION_7  310.45f
#define COURSE_REG_7      610.00f
#define COURSE_MISC_7     1299.85f
#define COURSE_LAB_7      2017.45f

#define COURSE_CODE_8     "Code8"
#define COURSE_DESC_8     "Course8"
#define COURSE_TUITION_8  399.79f
#define COURSE_REG_8      624.00f
#define COURSE_MISC_8     1349.75f
#define COURSE_LAB_8      2036.90f

struct Course {
    char courseCode[MAX_COURSE_LEN];
    char courseDescription[MAX_COURSE_DESC];
    float tuitionPerUnit;
    float registrationFee;
    float miscellaneousFee;
    float laboratoryFee;
};

/* Course database using constants (single definition) */
static struct Course courses[] = {
    { COURSE_CODE_1, COURSE_DESC_1, COURSE_TUITION_1, COURSE_REG_1, COURSE_MISC_1, COURSE_LAB_1 },
    { COURSE_CODE_2, COURSE_DESC_2, COURSE_TUITION_2, COURSE_REG_2, COURSE_MISC_2, COURSE_LAB_2 },
    { COURSE_CODE_3, COURSE_DESC_3, COURSE_TUITION_3, COURSE_REG_3, COURSE_MISC_3, COURSE_LAB_3 },
    { COURSE_CODE_4, COURSE_DESC_4, COURSE_TUITION_4, COURSE_REG_4, COURSE_MISC_4, COURSE_LAB_4 },
    { COURSE_CODE_5, COURSE_DESC_5, COURSE_TUITION_5, COURSE_REG_5, COURSE_MISC_5, COURSE_LAB_5 },
    { COURSE_CODE_6, COURSE_DESC_6, COURSE_TUITION_6, COURSE_REG_6, COURSE_MISC_6, COURSE_LAB_6 },
    { COURSE_CODE_7, COURSE_DESC_7, COURSE_TUITION_7, COURSE_REG_7, COURSE_MISC_7, COURSE_LAB_7 },
    { COURSE_CODE_8, COURSE_DESC_8, COURSE_TUITION_8, COURSE_REG_8, COURSE_MISC_8, COURSE_LAB_8 }
};
static const int noOfCourses = sizeof(courses) / sizeof(courses[0]);

struct Student {
    int id;
    char firstname[MAX_NAME_LEN];
    char lastname[MAX_NAME_LEN];
    char middlename[MAX_NAME_LEN];
    char courseCode[MAX_COURSE_LEN];
    char gender; /* 'M' or 'F' */
    int yearLevel;
    int noOfSubjects;
};

/* Utility: read a line from stdin into buf (removes trailing newline) */
static void readLine(char *buf, size_t size) {
    if (fgets(buf, (int)size, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    buf[strcspn(buf, "\n")] = '\0';
}

/* Utility: read integer with validation and range checking */
static bool readIntInRange(const char *prompt, int min, int max, int *outVal) {
    char line[128];
    long val;
    char *endptr;
    while (1) {
        printf("%s", prompt);
        readLine(line, sizeof(line));
        if (line[0] == '\0') {
            printf("Input cannot be empty. Please try again.\n");
            continue;
        }
        val = strtol(line, &endptr, 10);
        if (*endptr != '\0') {
            printf("Invalid number. Please enter an integer.\n");
            continue;
        }
        if (val < min || val > max) {
            printf("Sorry. %s %ld is invalid. Please check your data.\n", prompt, val);
            continue;
        }
        *outVal = (int)val;
        return true;
    }
}

/* Utility: read positive integer with custom message for invalid */
static bool readId(const char *prompt, int *outVal) {
    char line[128];
    long val;
    char *endptr;
    while (1) {
        printf("%s", prompt);
        readLine(line, sizeof(line));
        if (line[0] == '\0') {
            printf("ID cannot be empty. Please try again.\n");
            continue;
        }
        val = strtol(line, &endptr, 10);
        if (*endptr != '\0' || val <= 0) {
            printf("Invalid ID. Please enter a positive integer.\n");
            continue;
        }
        *outVal = (int)val;
        return true;
    }
}

/* Utility: read gender char M/F */
static bool readGender(const char *prompt, char *outGender) {
    char line[8];
    while (1) {
        printf("%s", prompt);
        readLine(line, sizeof(line));
        if (line[0] == '\0') {
            printf("Gender cannot be empty. Please enter M or F.\n");
            continue;
        }
        char c = toupper((unsigned char)line[0]);
        if (c != 'M' && c != 'F') {
            printf("Invalid gender '%s'. Please enter M or F.\n", line);
            continue;
        }
        *outGender = c;
        return true;
    }
}

/* Find course by code (case sensitive exact match) */
static struct Course * findCourse(const char *code) {
    for (int i = 0; i < noOfCourses; ++i) {
        if (strcmp(courses[i].courseCode, code) == 0) {
            return &courses[i];
        }
    }
    return NULL;
}

/* E Prompt course code ug e validate kung ni exists */
static void readCourseCode(char *outCode, size_t size) {
    char buf[64];
    while (1) {
        printf("Course Code: ");
        readLine(buf, sizeof(buf));
        if (buf[0] == '\0') {
            printf("Course Code cannot be empty.\n");
            continue;
        }
        /* trim leading spaces */
        char *p = buf;
        while (*p && isspace((unsigned char)*p)) p++;
        if (*p == '\0') {
            printf("Course Code cannot be empty.\n");
            continue;
        }
        struct Course *c = findCourse(p);
        if (c == NULL) {
            printf("Course '%s' not found. Use option 4 to view available courses.\n", p);
            continue;
        }
        strncpy(outCode, p, size - 1);
        outCode[size - 1] = '\0';
        return;
    }
}

/* Input functions */
static void inputStudent(struct Student *student) {
    printf("\n--- Enter Student Information ---\n");
    readId("ID#: ", &student->id);

    printf("Firstname: ");
    readLine(student->firstname, MAX_NAME_LEN);
    while (student->firstname[0] == '\0') {
        printf("Firstname cannot be empty. Please enter firstname: ");
        readLine(student->firstname, MAX_NAME_LEN);
    }

    printf("Lastname: ");
    readLine(student->lastname, MAX_NAME_LEN);
    while (student->lastname[0] == '\0') {
        printf("Lastname cannot be empty. Please enter lastname: ");
        readLine(student->lastname, MAX_NAME_LEN);
    }

    printf("Middlename: ");
    readLine(student->middlename, MAX_NAME_LEN);
    /* middlename may be empty */

    readCourseCode(student->courseCode, MAX_COURSE_LEN);

    readGender("Gender (M/F): ", &student->gender);

    readIntInRange("Year Level: ", MIN_YEAR_LEVEL, MAX_YEAR_LEVEL, &student->yearLevel);

    readIntInRange("No. of Subjects: ", MIN_SUBJECTS, MAX_SUBJECTS, &student->noOfSubjects);
}

/* Compute fees and display with required formatting */
static void displayStudentWithFees(const struct Student *student) {
    struct Course *course = findCourse(student->courseCode);
    if (course == NULL) {
        printf("\nCourse not found for student ID %d.\n", student->id);
        return;
    }

    int totalUnits = student->noOfSubjects * UNITS_PER_SUBJECT;
    float tuitionPerUnit = course->tuitionPerUnit;
    float totalTuition = (float)totalUnits * tuitionPerUnit;
    float registrationFee = course->registrationFee;
    float miscellaneousFee = course->miscellaneousFee;
    float laboratoryFee = course->laboratoryFee;
    float totalFees = registrationFee + miscellaneousFee + laboratoryFee;
    float totalAmount = totalTuition + totalFees;

    float prelimAssessment = totalAmount * PRELIM_PCT;
    float midtermAssessment = totalAmount * MIDTERM_PCT;
    float semiFinalAssessment = totalAmount * SEMIFINAL_PCT;
    float finalAssessment = totalAmount;

    printf("\n========================================\n");
    printf("        STUDENT ACCOUNTING DETAILS\n");
    printf("========================================\n");
    printf("ID#: %d\n", student->id);
    printf("Name: %s %s %s\n", student->firstname, student->middlename, student->lastname);
    printf("Gender: %c\n", student->gender);
    printf("Year Level: %d\n", student->yearLevel);
    printf("No. of Subjects: %d\n", student->noOfSubjects);

    printf("\n--- Course Information ---\n");
    printf("Course Code: %s\n", course->courseCode);
    printf("Course Description: %s\n", course->courseDescription);
    printf("Total Units: %d\n", totalUnits);
    printf("Tuition per Unit: P%.2f\n", tuitionPerUnit);
    printf("Total Tuition: P%.2f\n", totalTuition);

    printf("\n--- Fees ---\n");
    printf("Registration Fee: P%.2f\n", registrationFee);
    printf("Miscellaneous Fee: P%.2f\n", miscellaneousFee);
    printf("Laboratory Fee: P%.2f\n", laboratoryFee);
    printf("Total Fees: P%.2f\n", totalFees);

    printf("\n--- Payment Schedule ---\n");
    printf("Total Amount Due: P%.2f\n", totalAmount);
    printf("Prelim Assessment (53%%): P%.2f\n", prelimAssessment);
    printf("Midterm Assessment (64%%): P%.2f\n", midtermAssessment);
    printf("Semi-Final Assessment (75%%): P%.2f\n", semiFinalAssessment);
    printf("Final Assessment (Full Payment): P%.2f\n", finalAssessment);
    printf("========================================\n");
}

static void displayAllCourses(void) {
    printf("\n--- Available Courses ---\n");
    for (int i = 0; i < noOfCourses; ++i) {
        printf("%d. %s - %s (P%.2f/unit) Reg: P%.2f Misc: P%.2f Lab: P%.2f\n",
               i + 1,
               courses[i].courseCode,
               courses[i].courseDescription,
               courses[i].tuitionPerUnit,
               courses[i].registrationFee,
               courses[i].miscellaneousFee,
               courses[i].laboratoryFee);
    }
}

/* Find student index by ID, return -1 if not found */
static int findStudentById(const struct Student *students, int count, int id) {
    for (int i = 0; i < count; ++i) {
        if (students[i].id == id) return i;
    }
    return -1;
}

int main(void) {
    struct Student students[MAX_STUDENTS];
    int studentCount = 0;
    char line[16];

    printf("STUDENT ACCOUNTING SYSTEM\n");

    while (1) {
        printf("\n--- Menu ---\n");
        printf("1. Add Student\n");
        printf("2. View Student Details & Fees (by ID)\n");
        printf("3. View All Students\n");
        printf("4. View Available Courses\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        readLine(line, sizeof(line));
        if (line[0] == '\0') {
            printf("Invalid choice. Please enter a number 1-5.\n");
            continue;
        }
        int choice = (int)strtol(line, NULL, 10);
        switch (choice) {
            case 1:
                if (studentCount >= MAX_STUDENTS) {
                    printf("Maximum students reached!\n");
                    break;
                }
                inputStudent(&students[studentCount]);
                /* prevent duplicate IDs */
                if (findStudentById(students, studentCount, students[studentCount].id) != -1) {
                    printf("Student ID %d already exists. Entry discarded.\n", students[studentCount].id);
                } else {
                    studentCount++;
                    printf("Student added successfully!\n");
                }
                break;
            case 2: {
                int searchId;
                if (!readId("Enter Student ID to view details: ", &searchId)) break;
                int idx = findStudentById(students, studentCount, searchId);
                if (idx == -1) {
                    printf("Student not found!\n");
                } else {
                    displayStudentWithFees(&students[idx]);
                }
                break;
            }
            case 3:
                if (studentCount == 0) {
                    printf("No students to display!\n");
                } else {
                    for (int i = 0; i < studentCount; ++i) {
                        displayStudentWithFees(&students[i]);
                    }
                }
                break;
            case 4:
                displayAllCourses();
                break;
            case 5:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice. Please enter a number 1-5.\n");
                break;
        }
    }
    return 0;
}