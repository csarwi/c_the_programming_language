#include <stdio.h>

int main(void)
{
    float celsius, fahrenheid;
    int min = 0, max = 300, step = 20;
    celsius = min;

    printf("Celsus | Fahrenheit\n");
    printf("-------------------\n");

    while (celsius <= max)
    {
        fahrenheid = 32.0 + (9.0 * celsius / 5.0);
        printf("%5.0f%8.2f\n", celsius, fahrenheid);
        celsius += step;
    }
}