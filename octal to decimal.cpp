#include <stdio.h>

int main(void) {
    int num, octal = 0, place = 1;

    printf("Enter a number to convert: ");
    scanf("%d", &num);

    int temp = num;
    while (temp > 0) {
        int remainder = temp % 8;          
        octal = octal + remainder * place; 
        place *= 10;                       
        temp /= 8;                        
    }

    printf("Decimal %d in Octal = %d\n", num, octal);

    return 0;
}
