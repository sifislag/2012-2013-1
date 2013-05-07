#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions1.h"
#include "functions2.h"
#include "functions3.h"
#include "functions4.h"

 /* Function that adds a new n*n sized board after each turn to host the next one.*/
int addnext(char ****b,int turn,char n){ /* Board is char ****b so that it can change the adress of the char ***b main uses.*/
    b[0]=realloc(b[0],(turn+1)*sizeof(char**)); /*Reallocating b[0] to have enough space for the next turn. */
    if (b[0]==NULL){ /*Checking if it was succesful. */
        perror("Malloc");
        return 0;
    }

    b[0][turn]=malloc(n*sizeof(char*)); /*Allocating the n*n board b[0][turn] points to. */
    if (b[0][turn]==NULL){ 
        perror("Malloc");
        return 0;
    }
    int i,j;
    for(i=0;i<n;i++){ /*Allocating each line of the new n*n board. */
        b[0][turn][i]=malloc(n*sizeof(char));
        if(b[0][turn][i]==NULL){
            perror("Malloc");
            return 0;
        }
        for(j=0;j<n;j++)
            b[0][turn][i][j]=b[0][turn-1][i][j]; /*Initialising the board,copying the previous turn's board. */
    }
    return 1;
}

/* Function that is used to add the first turn.Used when begining a new game or when loading one. */

int addnew(char ****b,char n){ /*Again b is char**** so that the char*** b on main can have its address changed. */
    b[0]=malloc(sizeof(char **)); /*Adds a variable pointing to the first turn's n*n board. */
    if (b[0]==NULL){  /*Error check. */
        perror("Malloc");
        return 0;
    }
    b[0][0]=malloc(n*sizeof(char *)); /*Allocating the board pointing to each line. */
    if (b[0][0]==NULL){
        perror("Malloc");
        return 0;
    }
    int i,j;
    for(i=0;i<n;i++){ /*Allocating each new line. */
        b[0][0][i]=malloc(n*sizeof(char));
        if(b[0][0][i]==NULL){
            perror("Malloc");
            return 0;
        }
        for(j=0;j<n;j++)b[0][0][i][j]=' '; /*Initialising every object's value as ' '. */
    }
    return 1;
}

/*Variables explanations turn:starts from 0,each move increases it by one;diff:difficulty level;swap:if swap==1 swap rule is on;h to help with determining who's turn it is, (turn+h)%2==1 means its the player's turn(0/1 if player/computer is white);bload:1 if a game with black playing next has been loaded. */

int main(int argc,char *argv[]){
    int i=0,j=0,turn=0,diff=3,swap=0,k,h=1,bload=0;
    char human[]="white",computer[]="black";
    char ***b;
    char n=11;
    
    for(i=1;i<argc;i++){ /*Will make changes according to the commands given when the program was called. */
        if(!(strcmp(argv[i],"-s"))) /*Enable swap rule. */
            swap=1;
        else if(!(strcmp(argv[i],"-b"))){ /* Player plays as black. */
            strcpy(human,"black");
            strcpy(computer,"white");
        }
        else if(!(strcmp(argv[i],"-n"))){ /*Changes the size if input is appropriate. */
            /*Checks if there is something after "-n" in order not to go over the limits in case of wrong input.Also if the next word is a number. */
            if((i+1>=argc)||((*argv[i+1]<'0')||(*argv[i+1]>'9'))){
		fprintf(stderr,"Not given a new size after \"-n\".\nShutdown.\n");
		return 0;
	    }
	    if((atoi(argv[i+1])>=4)&&(atoi(argv[i+1])<=26)) /*If the given size has an acceptable value. */
                n=atoi(argv[++i]);
            else {
                fprintf(stderr,"Unacceptable size input(should be 4=<size=<26),shutdown.\n");
                return 0;
            }
        }
        else if(!(strcmp(argv[i],"-d"))){ /*Change the difficulty,if given level is appropriate */
	    if((i+1>=argc)||((*argv[i+1]<'0')||(*argv[i+1]>'9'))){ /*Checks if there is something after "-d" in order not to go over the limits in case of wrong input. */
		fprintf(stderr,"Not given a new difficulty level after \"-d\".\nShutdown.\n");
		return 0;
	    }
            if(atoi(argv[i+1])>0) /*If the new difficulty is over 1. */
                diff=atoi(argv[++i]);
            else {
                fprintf(stderr,"Unacceptable difficulty level input(should be >0),shutdown.\n");
                return 0;
            }
        }
        else /*If unknown parameter */
            fprintf(stderr,"Unacceptable parameters given.Program keeps running.Feel free to \"quit\" and start again.\n");
    }
    
    if(!(addnew(&b,n))) /*Beginning the game */
        return -1; 

    printf("Difficulty:%d\n",diff);
    printf("Size:%d\n",n);
    addnext(&b,++turn,n); /*Increasing turn and adding the turn's board. */
     /*Checks if the first digit of the human string is 'w'(means that human="white")and giving the appropriate value to h. */
    if (*human=='w')h=0;
    else h=1;
    
    /*Doing it so that for turn=0 the board is full of ' '.For turn=1 board is initially the same but changes before the next one is added. */
    
    /*Loop ends when a player gives a "quit" command or the functions encounter an error. */
    while(1){
        if((turn+h)%2){ /*If its the player's/computer's turn print the appropriate message. */
            printf("%s player(human) plays now.\n",human);    
        }
        else{
            printf("%s player(computer) plays now.\n",computer);            
        }
        printf("Turn:%d\n",turn);
        printgame(n,b,turn); /*Print the game status after each move. */
        
        do{                           /*Calls stringread until it returns 2(play/cont) */
            if(done(n,b,human,computer,turn-1,h)){
                turn=-1;
            }
            
            
            printf(">");
            k=stringread(&b,&turn,&diff,&n,human,computer,&swap,&h,&bload);
            if (k==0){                       /*If it returns 0 the program has encountered an error and needs to be terminated. */
                fprintf(stderr,"Fatal error.\n");
                return -1;
            }else if(k==-1)
                break;
            
        }while(k !=2);
        if(k==-1) /*In that case player quit. */
            break;
        
        if(!((turn+h)%2)){ /*If its the opponents turn calls the AI function,plays the move and prints the appropriate message */
            
            if(AI(n,diff,turn,b,NULL,&i,&j,1,*computer,*human,-1200,swap)){ /*1 is returned if*/
                swapit(b,n,turn);
            }
            else{
                b[turn][i][j]=*computer;
                printf("Move played: %c%d\n",'A'+j,i+1);
            }
        }
        
        if(!(addnext(&b,++turn,n)))
            return -1;
    }
    for (i=0;i<turn;i++){      /*Free what needs to be freed. */      
            for(j=0;j<n;j++)
                free(b[i][j]);
            free (b[i]);
    }
    free(b);
    printf("Application shut down.\n");
    return 1;
        
}


