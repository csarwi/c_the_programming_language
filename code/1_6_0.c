#include <stdio.h>
#include <stdlib.h>

int main(void)
{

    int c;
    int d;
    while ((c = getchar()) != EOF)
    {
        putchar(c);
        printf(" = %d\n", c);
    }
    printf("\n\n And the Last Value of c is %d", c);

    printf("\n\nUnfortunately, you're not done yet :D We have to do exercise 1-6 and verify that d = getchar() != EOF only gives 0 or 1;");

    char char_repr_of_c;
    while (c = getchar() != EOF)
    {
        char_repr_of_c = 48 + c;
        putchar(char_repr_of_c);
    }

    char_repr_of_c = 48 + c;
    printf("and now we are out and have %c", char_repr_of_c);
}