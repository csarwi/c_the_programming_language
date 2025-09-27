#include <stdio.h>

int main(void)
{
    char temp_input_char;

    while ((temp_input_char = getchar()) != EOF)
    {

        switch (temp_input_char)
        {
        case '\t':
            printf("\\t");
            break;

        case '\\':
            printf("\\\\");
            break;

        case '\b':
            printf("\\b");
            break;

        default:
            putchar(temp_input_char);
            break;
        }
    }
}
