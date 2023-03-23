#include <stdio.h>

int main()
{
    int n, i;
    int *pList;
    printf("Ingrese el tamaño del array: ");
    scanf("%d", &n);
    
    int listaDisp[n];
    
    for(i = 0; i < n; i++)
    {
        printf("Ingrese el elemnto #%d de la lista: ", i+1);
        scanf("%d",&listaDisp[i]);
    }
    
    pList = &listaDisp[0];
    
    for(i = 0; i < n; i++)
    {
        printf("El valor #%d por 2 es %d\n",i+1,(*pList)*2);
        pList++;
    }
    
    return 0;
}
