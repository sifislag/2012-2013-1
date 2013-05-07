#include <stdio.h>
#include <stdlib.h>
#include "functions.h"



int main(void){
    int n,m,i,j; /*Declaring variables for :board's sizes ,2 counters. */
    int** p;     /*Declaring the board */
    scanf("%d %d",&n,&m); /*Reads number of lines and columns */
    p=malloc(n*sizeof(int *)); /*Commiting the "pointer" part of the board */
    if(p==NULL){ /*Checking if it was succesful */
        printf("Error,Not enough memory available.");
        return 1;
    }
    for (i=0;i<n;i++){
        p[i]=malloc(m*sizeof(int));  /*Finish commiting the board. */
        if (p[i]==NULL){ /*Checking if it was succesful */
            printf("Error,Not enough memory available.");
            return 1;
        }
        for(j=0;j<m;j++){ /*Reads each line of the board after finishing commiting.*/
            scanf("%d",&p[i][j]);
        }
    }
    solve(n,m,p); /*Calls solve,which, according to what we define, chooses one of the three ways to run. */
    for (i=0;i<n;i++){ /*Freeing each line */
        free(p[i]);
    }
    free(p); /*Freeing the "pointer" part of the board */
        
}
