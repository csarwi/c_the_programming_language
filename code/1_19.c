#include <stdio.h>
#define MAXLINE 1000

int line_get(char line[], int max_length_of_line);
int line_reverse(char original[], char reversed[]);

int main(void)
{
    char line[MAXLINE];
    char reversed[MAXLINE];
    int line_length;

    while (line_get(line, MAXLINE) > 0)
    {

        line_reverse(line, reversed);

        for (int i = 0; reversed[i] != '\n'; i++)
        {
            putchar(reversed[i]);
        }
        putchar('\n');
    }
}

int line_get(char line[], int max_length_of_line)
{
    int c, i;

    for (i = 0; i < max_length_of_line - 1 && (c = getchar()) != EOF && c != '\n'; i++)
    {
        line[i] = c;
    }

    if (c == '\n')
    {
        line[i] = c;
        i++;
    }

    line[i] = '\0';
    return i;
}

int line_reverse(char original[], char reversed[])
{
    int array_length;
    for (array_length = 0; original[array_length] != '\n'; array_length++)
        ;

    int i, y;
    y = 0;
    for (i = array_length - 1; i >= 0; i--)
    {
        reversed[y] = original[i];
        y += 1;
    }
    reversed[y] = '\n';
}