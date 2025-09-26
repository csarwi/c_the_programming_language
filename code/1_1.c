// Exercise 1-1: Run the "Hello, World" program on yous system. Experiment with leaving out parts of the program, to see what error messages you get.

#include <stdio.h>

// MISSING SEMICOLON

// error C2143: syntax error: missing ';' before '}'

// int main()
//{
// printf("Hello, World!")
//}

// MISSING CLOSING "
// C:\src\c_the_programming_language\code\1_1.c(21): error C2001: newline in constant
// C:\src\c_the_programming_language\code\1_1.c(22): error C2143: syntax error: missing ')' before '}'
//
// int main(void)
//{
// printf("Hello, World!);
//}

int main(void)
{
    return;
}