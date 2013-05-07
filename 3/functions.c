#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#define PUREREC


int max(int a,int b,int c){ /*Function the finds maximum number out of three. */
    int max;
    max=a;
    if(max<b)max=b;
    if(max<c)max=c;
    return max; 
}
void pathfinder(int **p,int **b,int jmax,int n, int m){ /*Function that finds,and prints the path of the maximum sum,using the sum board.For the 2nd and 3rd implementations. */
    int i,j;
    printf("%d",p[0][jmax]);
    for (i=0;i<n-1;i++){       /*Will print path from 2nd to nth line.  */
        
        if(jmax==0){           /*If on the left edge of the board. */
            if (b[i+1][m-1]==max(b[i+1][m-1],b[i+1][0],b[i+1][1]))jmax=m-1;
            else if (b[i+1][1]==max(b[i+1][m-1],b[i+1][0],b[i+1][1]))jmax++;
            
        }else if(jmax==m-1){             /*If on the right edge of the board. */
            if (b[i+1][m-2]==max(b[i+1][m-2],b[i+1][m-1],b[i+1][0]))jmax--;
            else if (b[i+1][0]==max(b[i+1][m-2],b[i+1][m-1],b[i+1][0]))jmax=0;
            
        }else{                   /*Everywhere else */
            if (b[i+1][jmax-1]==max(b[i+1][jmax-1],b[i+1][jmax],b[i+1][jmax+1]))jmax--;
            else if (b[i+1][jmax+1]==max(b[i+1][jmax-1],b[i+1][jmax],b[i+1][jmax+1]))jmax++;
            
        } /*On these 3 conditions I'm checking if the highest sum on the next line is not on the same column with the previous max. */
        printf(" -> %d",p[i+1][jmax]);
    }
    printf("\n");
}

#ifdef PUREREC
void solve(int n,int m, int **p){
    printf("Running pureRecursive\n");
    int j,k,max; /*Declaring a variable to help with the repetitions, a variable to temporarily store the function's result,and one to store the maximum summary. */
    max=0;
    for (j=0;j<m;j++){ /*For each number of the first line,will calculate the summary using pureRecursive and then find the maximum summary. */
        k=pureRecursive(p,0,j,n,m);
        if(k>max)max=k;        
    }
    printf("Max sum is %d \n",max);
}
int pureRecursive(int ** p,int i,int j,int n, int m){ /*The way I write it may seem like theres a lot of useless code,but it prevents a lot of callings when it reaches the line before the last one(and is about 40% faster I think). */
    if (i==n-2){            /*When it reaches the line before the last it doesn't call itself again as, if it did, it would just return the board's value .*/
        if(j==0)        /*If on the far left column of the board */
            return p[i][j] +max(p[n-1][m-1],p[n-1][0],p[n-1][1]);
        
        else if(j==m-1)     /*If on the far right column of the board */
            return p[i][j] +max(p[n-1][m-2],p[n-1][m-1],p[n-1][0]);
        
        else          /*Everywhere else */
            return p[i][j] +max(p[n-1][j-1],p[n-1][j],p[n-1][j+1]);
    }            /*Before it reaches that line(0<=i<n-2) it does exactly what the function S does in the excercise. */
    
    else if(j==0)      /*If on the far left column of the board */
        return p[i][j]+max(pureRecursive(p,i+1,m-1,n,m),pureRecursive(p,i+1,0,n,m),pureRecursive(p,i+1,1,n,m));
    
    else if(j<m-1)     /*If not on the far left or far right columns. */
        return p[i][j]+max(pureRecursive(p,i+1,j-1,n,m),pureRecursive(p,i+1,j,n,m),pureRecursive(p,i+1,j+1,n,m));
    
    else               /*On the far right column of the board */
        return p[i][j] + max(pureRecursive(p,i+1,m-2,n,m),pureRecursive(p,i+1,m-1,n,m),pureRecursive(p,i+1,0,n,m)); 
}
#endif

#ifdef MATREC
void solve(int n,int m,int **p){
    printf("Running matrRecursive\n");
    int **b;      /*Declares a board to store the summary for each number of the board. */
    int i,j,smax=0,jmax;   /*Declares 2 variables for the loops,one for the maximum sum,and one to be used finding the path. */
    b=malloc(n*sizeof(int *));   /*Commiting the "pointer" part of the board */
    if (b==NULL){
        printf("Error,not enough memory available.");
        return;
    }
    for (i=0;i<n;i++){
        b[i]=malloc(m*sizeof(int)); /*Finish commiting the board. */
        if (b[i]==NULL){  
            printf("Error,not enough memory available.");
            return;
        }
        for(j=0;j<m;j++){   /*Initialise every object on the board as 0. */
            b[i][j]=0;
        }
    }
    for (j=0;j<m;j++){   /*Goes through the first line finding the maximum summary and where it is.*/
        if(matrRecursive(p,b,0,j,n,m)>smax){
            jmax=j;
            smax=b[0][jmax];
        }
    }
    printf("Max sum is %d.\n",smax);
    pathfinder(p,b,jmax,n,m);   /*Calling a function that finds and prints the path.*/
    for (i=0;i<n;i++){         /*Freeing the board.*/
        free(b[i]);
    }
    free(b);
}

int matrRecursive(int ** p,int ** b,int i,int j,int n, int m){ /*Didn't do it like pureRecursive(changing if i==n-2 to call itself less times),because it only reaches the bottom the first time(s) its called,the benefit would be too small and filling the bottom of the board would be harder.*/
    if (b[i][j]==0){             /*If the summary is 0 the function hasn't been called again for that part of the board,calculates it's value.*/
        if (i==n-1){             /*If it reached the bottom line.*/
            b[i][j]=p[i][j];
        } /*Else 0<=i<n-1 */
        else if(j==0)                 /*If on the far left column of the board */
            b[i][j]=p[i][j]+max(matrRecursive(p,b,i+1,m-1,n,m),matrRecursive(p,b,i+1,0,n,m),matrRecursive(p,b,i+1,1,n,m));
        
        else if(j<m-1)                 /*If not on the far left or far right column. */
            b[i][j]=p[i][j]+max(matrRecursive(p,b,i+1,j-1,n,m),matrRecursive(p,b,i+1,j,n,m),matrRecursive(p,b,i+1,j+1,n,m));
        
        else                            /*Else is on the far right column of the board */
            b[i][j]=p[i][j] + max(matrRecursive(p,b,i+1,m-2,n,m),matrRecursive(p,b,i+1,m-1,n,m),matrRecursive(p,b,i+1,0,n,m));
    }
    return b[i][j];         /*Returns the summary board's value/ */
}

#endif

#ifdef ITER
void solve(int n,int m,int **p){
   printf("Running iterative\n");
   int **b;                   /*Declares a board to store the summary for each number of the board. */
   int i,j,jmax,smax=0;        /*Declares 2 variables for the loops,one for the maximum sum,and one to be used finding the path. */
   b=malloc(n*sizeof(int *));  /*Commiting the "pointer" part of the board */
   if (b==NULL){
        printf("Error,not enough memory available.");
        return;
   }
   for (i=0;i<n;i++){        
       b[i]=malloc(m*sizeof(int));    /*Finish commiting the board. */
       if (b[i]==NULL){
           printf("Error,not enough memory available.");
           return;
       }
       for(j=0;j<m;j++){
           b[i][j]=p[i][j];     /*Initially copying the number board on the summary board to do less loops,not having to touch the last line. */
       }
   }
   for(i=n-2;i>=0;i--){     /*Starts from the bottom upwards,and ,using iterative, fills the summary board. */
       for (j=0;j<m;j++){
           iterative(b,i,j,m);
       }       
   }
   for (j=0;j<m;j++){       /*Finds the maximum summary and it's position using the summary board. */
        if(b[0][j]>smax){
            jmax=j;
            smax=b[0][jmax];
        }
    }
    printf("Max sum is %d.\n",smax);
    pathfinder(p,b,jmax,n,m);/*Calls a function to find and print the path. */
    for (i=0;i<n;i++){     /*Freeing the summary board. */
        free(b[i]);
    }
    free(b);
   
}   

int iterative(int ** b,int i,int j,int m){ /*Doesn't need to use p because when it begins s=p and once the objects on s are altered their old values aren't needed.*/
    if(j==0)        /*If on the far left column of the board */
        b[i][j]+=max(b[i+1][m-1],b[i+1][0],b[i+1][1]);
    
    else if(j<m-1)   /*If not on the far left or far right column. */
        b[i][j]+=max(b[i+1][j-1],b[i+1][j],b[i+1][j+1]);
    
    else      /*Else is on the far right column of the board */
        b[i][j]+= max(b[i+1][m-2],b[i+1][m-1],b[i+1][0]);
}
#endif
