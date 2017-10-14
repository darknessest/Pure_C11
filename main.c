#include <stdio.h>
#include <math.h>
#include <stdlib.h>

_Bool prime(unsigned long long a)
{
    for (unsigned long long i = 2; i < sqrt(a); i++) {
        if (a%i == 0)
            return 0;
        else
            return 1;
    }
}

int main()
{
    unsigned long long x = 0;
    scanf("%llu", &x);

    if (x > 3 && prime(x) == 1)
        printf("%llu is primary", x);
    else
        printf("error", x);
}