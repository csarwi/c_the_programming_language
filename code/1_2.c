#include <stdio.h>

int main(void)
{
    printf("I am finding out what happens when I print different escape characters\n");
    printf("There comes a tab\t for example. and here a backspace\b so there is only backspac even though i typed backspace and so on.\n");
    printf("and this one \\c does actually not exists so it prints just the letter \c. I was actually only able to print out \\c because i wrote \\\\c in the printf\n");
}