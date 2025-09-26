#include <stdio.h>

int main(void)
{
    float celsius, fahrenheid;
    int min = 0, max = 300, step = 20;
    fahrenheid = min;

    printf("F     |   C   \n");
    printf("------------\n");

    while (fahrenheid <= max)
    {
        celsius = (5.0 / 9.0) * (fahrenheid - 32.0);
        printf("%3.0f%9.2f\n", fahrenheid, celsius);
        fahrenheid += step;
    }
}