#include <stdio.h>

int main()
{
    float c, f;
    printf("ingrese ingrese la temperatura en farenheit:  ");
    scanf("%f", &f);
    c = (f-32)*(5/9);
    printf("la temperatura %f°F es igual a %f °C", f, c);
    return 0;
}
