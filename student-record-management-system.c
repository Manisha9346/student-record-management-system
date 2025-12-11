#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[50];
    char course[20];
    float gpa;
    int semester;
} Student;

#define DATAFILE "students.dat"
#define TEMPFILE "temp.dat"

void readLine(char *buffer, int size) {
    if (fgets(buffer, size, stdin)) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
            buffer[len - 1] = '\0';
        else {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
        }
    } else {
        buffer[0] = '\0';
    }
}

FILE* openStudentFile(const char *mode) {
    FILE *fp = fopen(DATAFILE, mode);
    if (!fp) perror("File error");
    return fp;
}

void addStudent() {
    Student s;
    FILE *fp = openStudentFile("ab");
    if (!fp) return;

    printf("Enter student ID: ");
    if (scanf("%d", &s.id) != 1) {
        while (getchar() != '\n');
        fclose(fp);
        return;
    }
    while (getchar() != '\n');

    printf("Enter name: ");
    readLine(s.name, sizeof(s.name));

    printf("Enter course: ");
    readLine(s.course, sizeof(s.course));

    printf("Enter GPA: ");
    scanf("%f", &s.gpa);

    printf("Enter semester: ");
    scanf("%d", &s.semester);
    while (getchar() != '\n');

    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);
    printf("Student added successfully.\n");
}

void displayAllStudents() {
    Student s;
    FILE *fp = openStudentFile("rb");
    if (!fp) return;

    int found = 0;
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        printf("\nID: %d\n", s.id);
        printf("Name: %s\n", s.name);
        printf("Course: %s\n", s.course);
        printf("GPA: %.2f\n", s.gpa);
        printf("Semester: %d\n", s.semester);
        printf("-------------------------\n");
        found = 1;
    }

    if (!found) printf("No records found.\n");
    fclose(fp);
}

void searchStudent() {
    Student s;
    int id;
    FILE *fp = openStudentFile("rb");
    if (!fp) return;

    printf("Enter student ID to search: ");
    scanf("%d", &id);
    while (getchar() != '\n');

    int found = 0;
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.id == id) {
            printf("\nStudent found:\n");
            printf("ID: %d\n", s.id);
            printf("Name: %s\n", s.name);
            printf("Course: %s\n", s.course);
            printf("GPA: %.2f\n", s.gpa);
            printf("Semester: %d\n", s.semester);
            found = 1;
            break;
        }
    }

    if (!found) printf("Student not found.\n");
    fclose(fp);
}

void updateStudent() {
    Student s;
    int id;
    FILE *fp = openStudentFile("r+b");
    if (!fp) return;

    printf("Enter ID to update: ");
    scanf("%d", &id);
    while (getchar() != '\n');

    int found = 0;
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.id == id) {
            printf("Enter new name: ");
            readLine(s.name, sizeof(s.name));

            printf("Enter new course: ");
            readLine(s.course, sizeof(s.course));

            printf("Enter new GPA: ");
            scanf("%f", &s.gpa);

            printf("Enter new semester: ");
            scanf("%d", &s.semester);
            while (getchar() != '\n');

            fseek(fp, -sizeof(Student), SEEK_CUR);
            fwrite(&s, sizeof(Student), 1, fp);

            printf("Record updated successfully.\n");
            found = 1;
            break;
        }
    }

    if (!found) printf("Student not found.\n");
    fclose(fp);
}

void deleteStudent() {
    Student s;
    int id;

    FILE *fp = openStudentFile("rb");
    if (!fp) return;

    FILE *temp = fopen(TEMPFILE, "wb");
    if (!temp) {
        fclose(fp);
        return;
    }

    printf("Enter ID to delete: ");
    scanf("%d", &id);
    while (getchar() != '\n');

    int found = 0;
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.id == id) {
            found = 1;
            continue;
        }
        fwrite(&s, sizeof(Student), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    if (!found) {
        printf("Student not found.\n");
        remove(TEMPFILE);
        return;
    }

    remove(DATAFILE);
    rename(TEMPFILE, DATAFILE);

    printf("Record deleted successfully.\n");
}

int main() {
    int choice;

    do {
        printf("\n--- Student Record System ---\n");
        printf("1. Add Student\n");
        printf("2. View All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("0. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayAllStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 0: printf("Exiting...\n"); break;
            default: printf("Invalid choice.\n");
        }

    } while (choice != 0);

    return 0;
}
