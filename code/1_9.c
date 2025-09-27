#include <stdio.h>

int main(void)
{
    char temp_char;
    char previous_char = 0;

    while ((temp_char = getchar()) != EOF)
    {

        if (temp_char != ' ')
        {
            putchar(temp_char);
        }

        if (temp_char == ' ' && previous_char != temp_char)
        {
            putchar(temp_char);
        }

        previous_char = temp_char;
    }
}