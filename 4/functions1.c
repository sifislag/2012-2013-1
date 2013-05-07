#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions1.h"
#include "functions2.h"
#include "functions3.h"
#include "functions4.h"

int addnext(char ****b,int turn,char n);
int addnew(char ****b,char n);
/*Function that reads and executes the player's ingame commands. */

int stringread(char ****b,int *turn,int *diff,char *n,char *human,char *computer,int *swap,int *h,int *bload){
    char str[30],sboard[5][10],*p; /*New string will be saved here. */
    fgets(str,sizeof(str),stdin); /*Using fgets to read it. */
    str[strlen(str)-1]='\0'; /*Turning the last digit of the string from '\n' to '\0' */
    if(*str=='\0'){
        printf("Empty string.Retry.\n");
        return 1;
    }
    int k=0;
    p=strtok(str,"      ");
    strcpy(sboard[k],p);
    while(p!=NULL){/*Using strtok recursively to chop str to pieces.*/
        p=strtok(NULL,"         ");
        if(p!=NULL)
            strcpy(sboard[++k],p);
    }
    
    if(!(strcmp(sboard[0],"cont"))){ /*If string is "cont".*/
        if(*turn==-1){ 
            printf("Game ended,start a new one or quit.\n");
            return 1;
        }
        
        if(((*turn)+(*h))%2){ /*If its the player's turn returns 1 and prints a messege notifying the player that he has to play. */
            printf("Have to play,\n");
            return 1;
        }
        return 2;                          /*Else returns 2 ,letting the name continue. */
    }else if(!(strncmp(sboard[0],"level",5))){       /*If the first 5 digits of the string are "level". */ 
        if(k==0){              /* If no 2nd word was given. */
            printf("Current difficulty level %d.\n",*diff);
            return 1;
        }
        else {                           /*Else get the new difficulty level using atoi on the 2nd */
            if(atoi(sboard[1])>=1){
                *diff=atoi(sboard[1]);
            }
            else{
                printf("Unacceptable difficulty level given.\n");
                return 1;
            }
            printf("New difficulty level:%d\n",*diff);
            return 1;
        }
    }else if(!(strcmp(sboard[0],"swap"))){     /*If string is "swap". */
        if((*turn==2)&&(*human=='b')&&(*swap)){ /*If player plays as black and its the 2nd turn(computer can call swap directly from the AI). */
            swapit(b[0],*n,*turn); 
            return 2;
        }
        else { 
            printf("swap command requires second player's first turn(and swap option to be enabled).");
            return 1;
        }
    }else if(!(strcmp(sboard[0],"suggest"))){ /*If string is "suggest" calls the AI function and prints the suggestion. */
        if(*turn==-1){
            printf("Game ended,start a new one or quit.\n");
            return 1;
        }
        if(!(((*turn)+(*h))%2)){
            printf("Not your turn.\n");
            return 1;
        }
        int i=0,j=0;
        if(AI(*n,*diff,*turn,b[0],NULL,&i,&j,1,*human,*computer,-1200,*swap))
            printf("Suggested move:swap\n");
        else
            printf("Suggested move:%c%d\n",'A'+j,i+1);
        return 1;
    }else if(!(strcmp(sboard[0],"quit"))){ /*If string is "quit" returns 0 making the game quit. */
        return -1;
    }else if(!(strcmp(sboard[0],"newgame"))){   /*If string begins with "newgame" */
        int i,j;
        for (i=0;i<*turn;i++){     /*Frees what needs to be freed. */       
            for(j=0;j<*n;j++)free(b[0][i][j]);
            free (b[0][i]);
        }
        free (b[0]);
        
        
        if(k>0){  /*if the string is bigger than "newgame" */
            if(!(strcmp(sboard[1],"white"))){ /*If 2nd word is "white" or "black". */
                strcpy(human,"white");
                strcpy(computer,"black");
                *h=0;
            }
            else if(!(strcmp(sboard[1],"black"))){
                strcpy(human,"black");
                strcpy(computer,"white");
                *h=1;
            }else{
                printf("Unacceptable color,options are white and black,color remains unchanged.\n");
            }
            if(k>1){ /*If the string is bigger than 2 words. */
                if(!(strcmp(sboard[2],"swapon"))) /*If 3rd word is "swapon" or "swapoff" */
                    *swap=1;
                else if(!(strcmp(sboard[2],"swapoff")))
                    *swap=0;
                else
                    printf("Unacceptable swap option given,options are swapon/swapoff ,setting remains unchanged");
                if(k>2){ /*If the string is bigger than 3 words. */
                    if((atoi(sboard[3])<1)||(atoi(sboard[3])>24))
                        printf("Unacceptable size given:%d.Size remains unchanged.\n",atoi(sboard[3]));
                    else
                        (*n)=atoi(sboard[3]);
                }
            }
        }
        if(!(addnew(b,*n))) /*Adds a new game board making sure it is succesfull. */
            return 0;
        
        
        *turn=1;
        if(!(addnext(b,*turn,*n))) /*Adds the next turn's board for reasons discused in main. */
            return 0;
        printgame(*n,b[0],*turn);
	return 1;
    }else if(!(strcmp(str,"undo"))){ /*If string is "undo". */
        if(*turn==-1){
            printf("Game ended,start a new one or quit.");
            return 1;
        }
        
        int i,j,k,l=(*turn+(*h))%2; /*Using l to determine how many times it needs to be undo'ed(one or two). */
        for(k=0;k<1+l;k++){
            if(*turn==1+ (*bload)){      /*If bload is 1(game has been loaded with black playing next) then it prevents the player from using undo if turn==2,otherwise if turn==1.*/
                printf("Cannot undo further.\n");
                break;
            }
            
            free(b[0][*turn]); /*Frees the n*n board of each turn it removes. */
            (*turn)--;
        }
 /*Gives the previous turn's value to the current(after undo) turn's board (will then add the player's move before the next turn's board is added). */
        for(i=0;i<*n;i++)
                for(j=0;j<*n;j++)
                    b[0][*turn][i][j]=b[0][(*turn)-1][i][j];
        printf("Turn:%d\n",*turn);
        printgame(*n,b[0],*turn);
        return 1;
    }else if(!(strcmp(sboard[0],"save"))){ /*If the first 4 letters of the string are "save". */
        if(strcmp(sboard[1]+strlen(sboard[1])-4,".hex")){ /*If the extension is not .hex */
            printf("File needs to be .hex\n");
            return 1;
        }
        
        int i,j;
        FILE *fp=fopen(sboard[1],"wb+"); /*Opens file(with name=strh) deleting everything previously stored there. */
        char *pt; /*The buffer to get the data into the files. */
        size_t sz=(2+(*n)*(*n))*sizeof(char); /*Number of bytes the new file will have */
        pt=malloc((2+(*n)*(*n))*sizeof(char)); /*Allocating memmory to first get them into the buffer. */
        pt[0]=*n; /*First byte is the size. */
        pt[1]=*computer; /*Second is the one who plays next. */
        if(((*turn)+(*h))%2)pt[1]=*human;
        for(i=0;i<*n;i++){          /*Getting each object of the board in the next n*n bytes of the buffer. */
            for(j=0;j<*n;j++){
                pt[2+i*(*n)+j]=b[0][*turn][i][j];
                if(pt[2+i*(*n)+j]==' ')pt[2+i*(*n)+j]='n'; /*It asked to save empty objects as 'n' */
            }
        }
        fwrite(pt,1,sz,fp); /*Writing the objects from the buffer to the file,then freeing the buffer and closing the file. */
        free(pt);
        fclose(fp);
        return 1;
    }else if(!(strcmp(sboard[0],"load"))){ /*If the first 4 letters of the string are "load". */
        if(strcmp(sboard[1]+strlen(sboard[1])-4,".hex")){ /*If the extension is not .hex */
            printf("File needs to be .hex\n");
            return 1;
        }
        
        
        int i,j;
        for (i=0;i<*turn;i++){            /*Free the 3 dimension board that hosted the game. */
            for(j=0;j<*n;j++)free(b[0][i][j]);
            free (b[0][i]);
        }
        free(b[0]);
        
        FILE *fp=fopen(sboard[1],"rb"); /*Opening a file to read binary */
        fread(n,1,1,fp); /*Reads the size and puts it in n. */
        char *pt;
        char next;
        fread(&next,1,1,fp); /*stores 'w' or 'b'(whoever plays next) on next after reading it. */
         /*Adding the 3 dimension board to host the game. */
        *turn=0;
        if(!(addnew(b,*n)))
            return 0;
        
        
        size_t sz=((*n)*(*n))*sizeof(char); /*n*n bytes going to be read next and get placed into the board. */
        pt=malloc(((*n)*(*n))*sizeof(char)); /*n*n sized buffer */
        if (pt==NULL)perror("Malloc");
        fread(pt,1,sz,fp); /*Reading from the file into the buffer. */
        for(i=0;i<*n;i++){  /*From the board into the buffer. */
            for(j=0;j<*n;j++){
                b[0][*turn][i][j]=pt[i*(*n)+j];
                if(b[0][*turn][i][j]=='n')b[0][*turn][i][j]=' '; /*'n' become ' ' again. */
            }
        }
        *turn=1; 
       if(!(addnext(b,*turn,*n))) /*Adding the next turn's board(checking if its successful). */
           return 0;
        *bload=0; /*Each time one loads bload variable becomes 0 again. */
        /*Had to implement the bload variable because if it was to start with black playing next(turn would have to be 2)
         * the undo command would be exploitable to make the player play more than once by undoing to turn=1. */
        if(next=='b'){  /*If black plays next.adds another turn and turns bload to 1. */
            *turn=2;
            if(!(addnext(b,*turn,*n)))
                return 0;
            *bload=1;
        }
        printgame(*n,b[0],*turn);
        return 1;
    }else if(!(strcmp(sboard[0],"cmd"))){ /*list of all available commands. */
        printf("Available commands:\nnewgame [white|black [swapoff|swapon [<size>]]]\nplay <move>\ncont\nundo\nsuggest\nlevel [<difficulty>]\nswap\nsave <statefile>\nload <statefile>\nshowstate\nquit\n");
        return 1;
    }else if(!(strcmp(sboard[0],"play"))){ /*If the first 4 letters are "play" */
        if(*turn==-1){
            printf("Game ended,start a new one or quit.\n");
            return 1;
        }
        if(k==0){ /*If string is only "play" */
            printf("You need to add coordinates in the following format:[capitalletter][number];example:play A3\n");
            return 1;
        }
        if(!(((*turn)+(*h))%2)){ /*If its your opponents turn. */
            printf("Wait for your turn.\n");
            return 1;
        }
        int j=(*(*(sboard+1)))-'A';  /*j is the 6th digit minus 'A'(For example for 'B' it would be 1) */
        int i=atoi(sboard[1]+1) -1; /*i is translation of the last part of the string into a number minus one. */
        if(!((sboard[1][1]>='0')&&(sboard[1][1]<='9'))){
            printf("Coordinates in wrong format(example \"play A1\").\n");
            return 1;
        }
        if((i<*n)&&(j<*n)){ /*If both are within limits of the board. */
            if (b[0][*turn][i][j]==' '){     /*If selected space is not already occupied */           
                b[0][*turn][i][j]=*human;   /*Gives the first letter of *human string('b' or 'w'). */
                printf("Move played: %c%d\n",'A'+j,i+1);
                return 2;
            }else{
                printf("Space occupied.\n");
                return 1;
            }
        }else{
            printf("Unacceptable coordinates.\n");
            return 1;
        }
    }else if(!(strcmp(sboard[0],"showstatus"))){ /*Showstatus command calling printgame function. */
        printf("Turn:%d\n",*turn);
        printgame(*n,b[0],*turn);
        return 1;
    }
    else{ /*Else not an acceptable command. */
        printf("Unknown command\"",str);
        int i;
        for(i=0;i<=k;i++)
            printf(" %s",sboard[i]);
        printf("\".\nType cmd to list all available commands.\n");
        
        return 1;
    }
}

/*Function that executes the swap move.*/
void swapit(char ***b,char n,int turn){
    int i,j;
    for (i=0;i<n;i++){
        for(j=0;j<n;j++){ /*Finds where 'w' is swaps it with the diametric move for 'b' and then returns*/
            if(b[turn][i][j]=='w'){
                b[turn][i][j]=' ';
                b[turn][j][i]='b';
                printf("Move played: swap\n");
                return;
            }
        }
    }
}

