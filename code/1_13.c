#include <stdio.h>

#define MAX_WORD_LENGTH 15
#define INSIDE_WORD 1
#define OUTSIDE_WORD 0

int main(void)
{
    int input_character, word_length, word_length_array[MAX_WORD_LENGTH];
    char state = OUTSIDE_WORD;
    input_character = word_length = 0;

    int i;
    for (i = 0; i < MAX_WORD_LENGTH; i++)
    {
        word_length_array[i] = 0;
    }

    while ((input_character = getchar()) != EOF)
    {
        if (input_character != ' ' && input_character != '\t' && input_character != '\n')
        {
            state = INSIDE_WORD;
            word_length++;
        }
        else
        {
            if (state)
            {
                word_length_array[word_length] += 1;
            }
            word_length = 0;
            state = OUTSIDE_WORD;
        }
    }

    // If the last word doesn't end with a whitespace it doesn't get counted, because during the word the if-statement in the while-loop is true and then when EOF comes the else block isn't executed anymore.
    if (state == INSIDE_WORD)
    {
        word_length_array[word_length]++;
    }

    printf("\n\nHere Comes the horizontal Histrogram, yeah, that way easy, even though the might make it nicer because it has overrun and the last word might not be counted.");
    for (i = 0; i < MAX_WORD_LENGTH; i++)
    {
        printf("%2d:", i);
        for (int y = 0; y < word_length_array[i]; y++)
        {
            putchar('-');
        }
        putchar('\n');
    }

    printf("\n\nHere Comes the vertical Histrogram\n\n");

    // print header

    for (i = 0; i < MAX_WORD_LENGTH; i++)
    {
        printf("%d\t", i);
    }
    printf("\n");

    int highest_number = 0;

    for (i = 0; i < MAX_WORD_LENGTH; i++)
    {
        if (word_length_array[i] > highest_number)
        {
            highest_number = word_length_array[i];
        }
    }

    for (i = 1; i <= highest_number; i++)
    {

        for (int y = 0; y < MAX_WORD_LENGTH; y++)
        {
            if (word_length_array[y] >= i)
            {
                printf("|\t");
            }
            else
            {
                printf("\t");
            }
        }
        printf("\n");
    }

    return 0;
}