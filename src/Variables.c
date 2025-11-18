#include <stdio.h>
#include <string.h> // For strcat and strcpy

int main() {
    // Initialize variables
    char firstname[] = "Jundel ";
    char lastname[] = "Malazarte";
    char fullname[50]; 
    int age = 21;
    double height = 1.75;

    // Concatenate strings
    strcpy(fullname, firstname); // Copy firstname into fullname
    strcat(fullname, lastname); // Concatenate lastname to fullname

    // Print variables
    printf("Hello, %s\n", fullname);
    printf("Age: %d, Height: %.2f meters\n", age, height);

    return 0;
}
