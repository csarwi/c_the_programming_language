#include <stdio.h>

#define INSIDE_WORD 1
#define OUTSIDE_WORD 0

int main(void)
{
    int character, newline_counter, word_counter, character_counter, state;

    state = OUTSIDE_WORD;
    newline_counter = word_counter = character_counter = character = 0;

    while ((character = getchar()) != EOF)
    {
        ++character_counter;

        if (character == '\n')
        {
            ++newline_counter;
        }

        if (character == ' ' || character == '\n' || character == '\t')
        {
            state = OUTSIDE_WORD;
        }
        else if (state == OUTSIDE_WORD)
        {
            state = INSIDE_WORD;
            word_counter++;
        }
    }

    printf("NewLines: %d , Words: %d , Chars: %d\n", newline_counter, word_counter, character_counter);
}