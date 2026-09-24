#include <stdio.h>

int main() {
    int year;

    printf("Enter a year: ");
    scanf("%d", &year);

    // This expression results in 1 (True) for a leap year, and 0 (False) for a regular year
    int is_leap = (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);

    // Prints 1 if it is a leap year, or 0 if it is not
    printf("Leap year status (1 = Yes, 0 = No): %d\n", is_leap);

    return 0;
}
