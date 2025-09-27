#include <stdio.h>

int main(void)
{

    int input_character;

    while ((input_character = getchar()) != EOF)
    {
        if (input_character == '\n' || input_character == ' ' || input_character == '\t')
        {
            putchar('\n');
        }
        else
        {
            putchar(input_character);
        }
    }

    return 0;
}