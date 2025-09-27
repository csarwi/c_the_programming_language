#include <stdio.h>

int main(void)
{

    char temp_char;
    int tab_counter = 0, newline_counter = 0, space_counter = 0;

    while ((temp_char = getchar()) != EOF)
    {

        // Of course this could be written better with else if or even a switch-statement, but i'm doing what there is known
        // so far in the book

        if (temp_char == '\n')
            newline_counter++;

        if (temp_char == '\t')
            tab_counter++;

        if (temp_char == ' ')
            space_counter++;
    }

    printf("\nSpaces: %d\n", space_counter);
    printf("Tabs: %d\n", tab_counter);
    printf("Newlines: %d\n", newline_counter);
}