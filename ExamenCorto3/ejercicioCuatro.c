#include <stdio.h>
#include "add.h"

int main()
{
    int a, b;
    printf("Ingrese dos valores separados por un espacio para operar: ");
    scanf("%d %d", &a, &b);
    int c = add(a,b);
    int d = sub(a,b);
    int e = mul(a,b);
    int f = dev(a,b);
    float g = pi();
    printf("%d\n", c);
    printf("%d\n", d);
    printf("%d\n", e);
    printf("%d\n", f);
    printf("%f\n", g);

    return 0;
}
