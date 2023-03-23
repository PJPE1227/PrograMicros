#include <stdio.h>

int main()
{
    int i,j,k,fib,x;
    printf("Ingrese la cantidad de terminos que desea ver: ");
    scanf("%d", &j);
    k = 0;
    fib = 1;
    printf("%d ", 1);
    for (i = 1; i < j; i++){
        x = fib;
        fib = fib + k;
        k = x;
        
        printf("%d ", fib);        
    }
    return 0;
}
