
#include <stdio.h>
#include <stdlib.h>

int main()
{
    char snake[4][9]={" @ o oOOO","OOOo o @ "," oO@ O oO","Oo O @Oo "};
    for(int i=0;i<4;i++){
        for(int j=0;j<9;j++)
        {
            printf("%c",snake[i][j]);
            if((j+1)%3==0) printf("\n");
        }
        printf("\n\n");
    }
    return 0;
}