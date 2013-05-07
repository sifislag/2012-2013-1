#include <stdio.h>
#include "functions3.h"

/*
 Functions about printing the game board.
 
 */



/*Prints x amount of spaces.Used by printgame(). */
void sp(int x){
   int i;
   for(i=1;i<=x;i++,printf(" "));
}

/*Not gonna comment on this one,ugly code but almost identical to the excercise prototype,certainly functions as it should. */
void printgame(char n,char ***b,int turn){ 
    int i,j,k;
    char *str="BLACK ";
    sp(2*n);printf("W H I T E\n       ");
    for(j=0;j<n;j++)printf(" %c  ",j+65);
    printf("\n       ");
    for(j=0;j<n;j++)printf(" _  ");
    printf("\n       ");
    for(j=0;j<n;j++)printf("/ \\_");
    printf("\n");
    for(i=0;i<n/2-1;i++){
        sp(2*i+4);
        printf("%2d",i+1);
        for(j=0;j<n;j++)printf("| %c ",b[turn][i][j]);
        printf("| %d\n ",i+1);
        sp(2*i+6);
        for(j=0;j<n;j++)printf("\\_/ ");
        printf("\\");
        printf("\n");
    }
    k=0;
    for(i=n/2-1;i<n/2+2;i++){
        sp(2*i+2);
        printf("%c ",*(str+k));
        printf("%2d",i+1);
        for(j=0;j<n;j++)printf("| %c ",b[turn][i][j]);
        printf("| %d\n ",i+1);
        sp(2*i+2);
        printf("%c   ",*(str+k+1));
        for(j=0;j<n;j++)printf("\\_/ ");
        printf("\\");
        printf("\n");
        k+=2;
    }
    for(i=n/2+2;i<n;i++){
        sp(2*i+4);
        printf("%2d",i+1);
        for(j=0;j<n;j++)printf("| %c ",b[turn][i][j]);
        printf("| %d\n ",i+1);
        sp(2*i+6);
        for(j=0;j<n;j++)printf("\\_/ ");
        printf("\\");
        printf("\n");
    }
    fflush(stdout);
}
