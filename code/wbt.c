#include <stdio.h>

typedef struct
{
    int factorial;
    float root;
} FacRoot;

int square_factorial_root(FacRoot *result, int n)
{
    result->factorial = 1; // NOTE: this is still correct when n=0 or n=1

    for (int i = 2; i <= n; ++i)
    {
        result->factorial *= i;
    }

    result->root = sqrt(result->factorial);
    return n * n;
}

int main(void)
{

    FacRoot myFacRoot;
    square_factorial_root(&myFacRoot, 200);
}