#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions1.h"
#include "functions2.h"
#include "functions3.h"
#include "functions4.h"
#include "functions5.h"
/*
 Includes the "base" of the AI minimax and alphabeta pruning and 2 functions used to determine a statuses value by score()
 
 */

/*Using minimax with alpha-beta pruning on this one,not sure if I've understood alphabeta pruning correctly but it seems to be working(same results,less time)*/
int AI(char n,int diff,int turn,char ***b,char ***c,int *pi,int *pj,int depth,char caller,char enemy,int alphabeta,int swap){
    if(turn==1){ /*In the computer plays first then it chooses the move at the center */
        *pi=n/2;
        *pj=n/2;
        return 0;
    }
    /*If the computer plays second it steals the player's move using swap(as the one playing first always has an advantage in hex. */
    if((turn==2)&&(swap==1)){ 
        return 1;
    }
    
   /*Declaring counters,max(starting at -1001 as the worst possible outcome is -1000),min(same)*/
   /*gab starts at -1200 and when it changes(see later) it gives the next calling of AI alpha/beta for pruning.*/
    int i=0,j=0,ci=0,cj=0,max=-100001,min=100001,gab=-1200;
    /*Temporary score of each node and a secondary score used if 2 nodes have the same tscore==max*/
    int tscore=0,secscore=0;
    
    /*Allocating a 3D board :c[0]-c[diff-1](c[depth-1]), 2D boards are used as a base for c[depth] for each calling/iteration*/
    if(depth==1){
        c=malloc((diff+1)*sizeof(char **));
        if(c==NULL){
            perror("Malloc");
            return 0;
        }
    
        for(i=0;i<diff +1;i++){
            c[i]=malloc((n)*sizeof(char *));
            if(c[i]==NULL){
                perror("Malloc");
                return 0;
            }
            for(j=0;j<n;j++){
                c[i][j]=malloc((n)*sizeof(char));
                if(c[i][j]==NULL){
                    perror("Malloc");
                    return 0;
                }
            }
        }
        /*Copying the current status of the game to c[0]*/
        for(i=0;i<n;i++)
            for(j=0;j<n;j++)
                c[0][i][j]=b[turn][i][j];
    }
    /*Copying c[depth-1] to c[depth]*/
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            c[depth][i][j]=c[depth-1][i][j];
    
        
    
    
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            if(c[depth][i][j]==' '){ /*If the cell isn't occupied */
            /*Will be using depth%2 condition to determine if its a condition in which one calling the function(1,3,5...)/his enemy(2,4,6...) would move max/min would be selected etc*/
                if(depth%2){
                    c[depth][i][j]=caller;
                }
                else{
                    c[depth][i][j]=enemy;
                }
                
                if((max!=-1001)||(min!=1001)){ /*If either min or max has been altered this isn't the first node of this subtree. */
                    if(depth%2)
                        gab=max;
                    else
                        gab=min;
                }
                
            /*If at the the "bottom" of the tree uses score function to determine the worth of this move else calls AI for depth+1*/
                if(depth==diff){
                    tscore=score(c[depth],n,diff,turn,caller,enemy);
                }
                else{
                    tscore=AI(n,diff,turn,b,c,pi,pj,depth+1,caller,enemy,gab,swap);
                }
                
                if(alphabeta!=-1200){ /*If its not on the first depth level and the parent node already has a value(min/max) set(other than the default one). */
                    if(depth%2){  /*If its a level in which max is selected and the current max is bigger than min of the previous level */
                        if(alphabeta<max) /*There is no point in continuing on this sub tree as max can only get bigger and can't change min. */
                            return max;
                    }
                    else{ /*Same logic*/
                        if(alphabeta>min)
                            return min;
                    }
                }
                
                
                if (depth%2){
                    if(tscore>=max){
                        if(tscore==max){ /*If 2 nodes at the first depth have the same score(same outcome in time) picks the one with the best direct outcome */
                            if(depth==1)
                                if(score(c[1],n,diff,turn,caller,enemy)>secscore){
                                    secscore=score(c[1],n,diff,turn,caller,enemy);
                                    *pi=i;
                                    *pj=j;
                                }
                        }
                        else{ /*Change max and if on the first level the coordinates to be returned */
                            max=tscore;
                            if(depth==1){
                                *pi=i;
                                *pj=j;
                            }
                        }
                    }
                }else{
                    if(tscore<min)
                        min=tscore;
                }
                 /*c[depth] becomes c[depth-1] again as it prepares for the next nod*/       
                for(ci=0;ci<n;ci++)
                    for(cj=0;cj<n;cj++)
                        c[depth][ci][cj]=c[depth-1][ci][cj];
                       
            }
        }
    }
    /* If the iterations are over (for the first calling of AI) it frees the board and returns 0*/
    if(depth==1){
        for(i=0;i<diff+1;i++){
            for(j=0;j<n;j++){
                free(c[i][j]);
            }
            free(c[i]);
        }
        free(c);
        return 0; /*Returns 0 if it has finished calculating the move(already returned one if its a swap move).*/
        
    }
    /*Return the result to the previous calling of AI().*/
    if(depth%2)
        return max;
    else
        return min;
    
}

/*Used by score() to find the biggest chain of linked cells on our board */
/*Also finds the most "extreme cells of this chain(closer to an edge) and uses them to figure out if the chain is a winning one . */
/*and how spread the chain is(in the direction that it should be) */
void linkedscore(char **b,char n,int i,int j,int *score,int *imin,int *imax,int *jmin,int *jmax){
    /*If the chain is a white one imin,jmin/imax,jmax are the coordinates of the cell of the chain with the minimun/maximum i(jmin for example doesn't have to be the minimum j)*/
    /*Same logic if the chain is black*/
    if(b[i][j]=='w'){
        if(i<(*imin)){
            (*imin)=i;
            (*jmin)=j;
        }
        if(i>=(*imax)){
            (*imax)=i;
            (*jmax)=j;
        }
    }
    else{
        if(j<(*jmin)){
            (*jmin)=j;
            (*imin)=i;
        }
        if(j>=(*jmax)){
            (*jmax)=j;
            (*imax)=i;
        }
    }
    
    char temp=b[i][j]; /*Saving original value of the cell */
    b[i][j]=temp-1;/*Giving visited status to the cell,to prevent the function from going to an endless loop revisiting the same cells. */
    
    if(i!=0){  /*If not on the first line will check the object right above it. */
        if(temp==b[i-1][j]){ /*If they are of the same color. */
                (*score)+=2;
                linkedscore(b,n,i-1,j,score,imin,imax,jmin,jmax); 
        }
    }
    if(i!=n-1){ /*If not on the bottom checks the object right below it. */
        if(temp==b[i+1][j]){
                (*score)+=2;
                linkedscore(b,n,i+1,j,score,imin,imax,jmin,jmax);
            }
    }
    if(j!=0){ /*If not on the far left column will check the object on the left. */
        if(temp==b[i][j-1]){
                (*score)+=2;
                linkedscore(b,n,i,j-1,score,imin,imax,jmin,jmax);
        }
    }
    if(j!=n-1){ /*If not on the right column will check the object on the right. */
        if(temp==b[i][j+1]){
                (*score)+=2;
                linkedscore(b,n,i,j+1,score,imin,imax,jmin,jmax);
        }
    }
    if((i!=0)&&(j!=n-1)){  /*If not on the first line or far right column checks for the object above and on the right. */
        if(temp==b[i-1][j+1]){
                (*score)+=2;
                linkedscore(b,n,i-1,j+1,score,imin,imax,jmin,jmax);
        }
    }
    if((i!=n-1)&&(j!=0)){ /*If not on the last line or the far left column check for the object below and on the left. */
        if(temp==b[i+1][j-1]){
                (*score)+=2;
                linkedscore(b,n,i+1,j-1,score,imin,imax,jmin,jmax);
        }
    }
        
    return;
}


/*Function if there are blocks that can be linked in two possible ways(which is a good part of establishing my strategy)*/
void tobelinked(char **c,char color,char n,int i,int j,int *score){
    char encol='w';
    if (color=='w')
        encol='b';
    c[i][j]=color-1; /*Giving it visited status */
    if((i<n-1)&&(j<n-1)){ /*First is a pattern that is good for both */
        if((c[i+1][j]==encol)&&(c[i][j+1])==encol){
            (*score)-=4;
        }
        else{ 
            if((c[i+1][j]==encol)||(c[i][j+1])==encol){
                if((c[i+1][j]==color)||(c[i][j+1])==color)
                    (*score)+=2;
                else
                    (*score)-=2;
            }
            if(c[i+1][j+1]==color){
                (*score)+=4;
                tobelinked(c,color,n,i+1,j+1,score);
            }
        }
    }
    if (color=='w'){ /*This is good for the white player(could be used by the black if on a dead end) */
        if((i<n-2)&&(j>0)){
            if((i<n-1)&&(j>1)){
                if(c[i+1][j-2]=='b') /*If this cell blocks the opponents way(same for the next 3) */
                    (*score)+=4;
            }
            if((i>0)&&(j<n-2)){
            	if(c[i-1][j+2]=='b')
            	    (*score)+=4;
			}
			if((i>0)&&(j>0)){ /*Some of the conditions might be pointless but wanted it to look clear which it is to block */
            	if(c[i-1][j-1]=='b')
            	    (*score)+=2;
			}
			if((i<n-1)&&(j<n-1)){
            	if(c[i+1][j+1]=='b')
            	    (*score)+=2;
			}
            if((c[i+1][j]=='b')&&(c[i+1][j-1]=='b')){ /*If both are on the way of it linking with the one we've targeted return */
                (*score)-=4;
                return;
            }
            if((c[i+1][j]=='b')||(c[i+1][j-1]=='b')){ /*if only one block the way */
                if((c[i+1][j]=='w')||(c[i+1][j-1]=='w')) /*If the other one is making a bridge */
                    (*score)+=2;
                else
                    (*score)-=2;
            }
            if(color==c[i+2][j-1]){ /*If its the same color as the target,increase the score, calls itself again for the target's location */
                (*score)+=4;
                tobelinked(c,color,n,i+2,j-1,score);
            }
        }
        
    }
    else if(color=='b'){ /*This is good for the black player(could be used by the white if on a dead end) */
        if((i<n-1)&&(j>1)){
            if((i<n-2)&&(j>0)){
                if(c[i+2][j-1]=='w')
                    (*score)+=4;
            }
            if((i>1)&&(j<n-1)){
                if(c[i-2][j+1]=='w')
                    (*score)+=4;
            }
            if((i>0)&&(j>0)){ 
            	if(c[i-1][j-1]=='w')
            	    (*score)+=2;
			}
	    if((i<n-1)&&(j<n-1)){
            	if(c[i+1][j+1]=='w')
            	    (*score)+=2;
			}
            if((c[i][j-1]=='w')&&(c[i+1][j-1]=='w')){
                (*score)-=4;
                return;
            }
            if((c[i+1][j]=='w')||(c[i+1][j-1]=='w')){
                if((c[i+1][j]=='b')||(c[i+1][j-1]=='b'))
                    (*score)+=2;
                else
                    (*score)-=2;
            }
            if(color==c[i+1][j-2]){
                (*score)+=4;
                tobelinked(c,color,n,i+1,j-2,score);
            }
        }
    }
    
    return;
}
