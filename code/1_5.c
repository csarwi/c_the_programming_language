
#include <stdio.h>

int main(void)
{
    for (int fahrenheit = 300; fahrenheit >= 0; fahrenheit -= 20)
    {
        printf("%3d%9.2f\n", fahrenheit, (5.0 / 9.0) * (fahrenheit - 32));
    }
}