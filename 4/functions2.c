#include <stdio.h>
#include <stdlib.h>
#include "functions1.h"
#include "functions2.h"
#include "functions3.h"
#include "functions4.h"

/*Recursive function,calling itself when it finds an object with the same color(which it can link with) untill
 *  it reaches the bottom or there are no nearby objects with the same color. */
int linked(char **b,char n,int i,int j){
    if(i==n-1){ /*If it reached the bottom returns 1 */
        if (b[i][j]=='w') /*Starts printing the winning path(if its black then use i as j and j as i because the board is transposed.*/
            printf("Winning path: %c%d",'A'+j,i+1);
        else
            printf("Winning path: %c%d",'A'+i,j+1);
        return 1;
    }
    
    
    int h=0; /*Will store the next calling's result in h. */
    
    if(i!=0){  /*If not on the first line will check the object right above it. */
        if(b[i][j]==b[i-1][j]){ /*If they are of the same color. */
                b[i][j]--; /*Altering the object a bit so that linked() called in the next line doesn't call itself again for our current object. */
                h=linked(b,n,i-1,j); 
                b[i][j]++; /*Returns to the original value */
                if(h){
                    if(b[i][j]=='w')
                        printf("-%c%d",'A'+j,i+1);
                    else
                        printf("-%c%d",'A'+i,j+1);
                    return h;
                }
                
        }
    }
    if(i!=n-1){ /*If not on the bottom checks the object right below it. */
        if(b[i][j]==b[i+1][j]){
                b[i][j]--;
                h=linked(b,n,i+1,j);
                b[i][j]++;
                if(h){
                    if(b[i][j]=='w')
                        printf("-%c%d",'A'+j,i+1);
                    else
                        printf("-%c%d",'A'+i,j+1);
                    return h;
                }
            }
    }
    if(j!=0){ /*If not on the far left column will check the object on the left. */
        if(b[i][j]==b[i][j-1]){
                b[i][j]--;
                h=linked(b,n,i,j-1);
                b[i][j]++;
                if(h){
                    if(b[i][j]=='w')
                        printf("-%c%d",'A'+j,i+1);
                    else
                        printf("-%c%d",'A'+i,j+1);
                    return h;
                }
        }
    }
    if(j!=n-1){ /*If not on the right column will check the object on the right. */
        if(b[i][j]==b[i][j+1]){
                b[i][j]--;
                h=linked(b,n,i,j+1);
                b[i][j]++;
                if(h){
                    if(b[i][j]=='w')
                        printf("-%c%d",'A'+j,i+1);
                    else
                        printf("-%c%d",'A'+i,j+1);
                    return h;
                }
        }
    }
    if((i!=0)&&(j!=n-1)){  /*If not on the first line or far right column checks for the object above and on the right. */
        if(b[i][j]==b[i-1][j+1]){
                b[i][j]--;
                h=linked(b,n,i-1,j+1);
                b[i][j]++;
                if(h){
                    if(b[i][j]=='w')
                        printf("-%c%d",'A'+j,i+1);
                    else
                        printf("-%c%d",'A'+i,j+1);
                    return h;
                }
        }
    }
    if((i!=n-1)&&(j!=0)){ /*If not on the last line or the far left column check for the object below and on the left. */
        if(b[i][j]==b[i+1][j-1]){
                b[i][j]--;
                h=linked(b,n,i+1,j-1);
                b[i][j]++;
                if(h){
                    if(b[i][j]=='w')
                        printf("-%c%d",'A'+j,i+1);
                    else
                        printf("-%c%d",'A'+i,j+1);
                    return h;
                }
        }
    }
    return 0;
}

void transpose(char n, char **a){ /*On our second lab report we had to do a function like this so I altered it to fit this project. */
	int i,j;
	int **b; /*Second,temporary,2 dimension board. */
	b=malloc(n*sizeof(int *));
	for(i=0;i<n;i++){
		b[i]=malloc(n*sizeof(int));
		for(j=0;j<n;j++){
                    b[i][j]=a[i][j]; /*Copying the original to the temporary board. */
                }
	}
        for(i=0;i<n;i++){
            for(j=0;j<n;j++){
                a[j][i]=b[i][j]; /* Transposing the temporary board into the original one. */
            }
            free (b[i]); /* Freeing the temporary one */
        }
        free(b);
}
/*Function that checks if the previous turn was a winning turn*/
int done(char n,char ***b,char *human,char *computer,int turn,int h){
    int k=0;
    if (turn>=2*n - 1){ /* For turn<2n-1 not enough moves have been played to connect the two. edges. */
        int j;
        char col='w'; /*Col is the color the the function looks for in the iteration(if found calls linked()). */
   /*If black player played the turn just finished(would be next turn but my I use turn-1 when I call the function in main(). */
        if(((!((turn+h)%2))&&((*computer)=='b'))||(((turn+h)%2)&&((*human)=='b'))){
            transpose(n,b[turn]);       /*Transposing the board to have the same algorithm for both black and white. */
            col='b';           /*The color we're searching for is now black */
        }
        for(j=0;j<n;j++){
            if(b[turn][0][j]==col)
                if(linked(b[turn],n,0,j)){ /*If linked returns 1(which means it was successful) make k=1 and stop the iterations.*/
                    k=1;
                    printf("\n");
                    break;
            }
        }
        /*If the board was originally transposed it returns to it's original state. */
        if(((!((turn+h)%2))&&((*computer)=='b'))||(((turn+h)%2)&&((*human)=='b'))){ 
            transpose(n,b[turn]);
        }
    }
    
    if(k){ /*Winning/losing ASCII art. */
        if((turn+h)%2){
            printf(" __   __                     _       _ \n \\ \\ / /__  _   _  __      _(_)_ __ | |\n  \\ V / _ \\| | | | \\ \\ /\\ / / | '_ \\| |\n");
            printf("   | | (_) | |_| |  \\ V  V /| | | | |_|\n   |_|\\___/ \\__,_|   \\_/\\_/ |_|_| |_(_)\n");
        }
        else{
            printf(" __   __            _                _ \n \\ \\ / /__  _   _  | | ___  ___  ___| |\n  \\ V / _ \\| | | | | |/ _ \\/ __|/ _ \\ |\n");
            printf("   | | (_) | |_| | | | (_) \\__ \\  __/_|\n   |_|\\___/ \\__,_| |_|\\___/|___/\\___(_)\n");
        }
    }
    
    return k;
}

