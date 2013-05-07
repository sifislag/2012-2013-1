#include <stdio.h>
#include <ctype.h>
#define MINLENGTH 9
#define MINCRITERIA 3

int main(void){
int ch=0,x=0,clet=0,cnum=0,prch=-1,cc6=1;     /* Declaring character,character counter,counters for letters and numbers the previous character and a counter used on the 6th criterion. */
short low=0,upp=0,num=0,spe=0,una=0,twosame=1,threesuc=1;  /* Declaring logical variables for each of the criteria( to be ==1 if criterion is met) and one for unacceptable characters. */
short c5=0,c6=0; /*Since for the 5th/6th criteria I'm checking the last possible set of characters to work in case [enter] is pressed right after */
/*I'm declaring these two variables to make proper checks on the next loop */
ch = getchar();        /* Read first character */
while (ch != EOF) {    /* Go on if we didn't reach end of file */
    x++;
    if(ch>='a'&& ch<='z'){  /* If lower case letter */                   
        low=1;
        clet++;}
    else if(ch>='A'&& ch<='Z'){ /* If upper case letter */
        upp=1;
        clet++;}
    else if(ch>='0'&& ch<='9'){ /*If number */
        num=1;
        cnum++;}
    else if(ispunct(ch))spe=1; /*If special character */
    else una=1;     /* Everything else is unacceptable */
    
    
      
    if (prch==ch){ /*If 2 last characters are the same,arranges to check on the next loop (If there is one) and makes the logical variable 0. */
        if(twosame==1)c5=x+1; /*If twosame==0 already there's no point in making the check on the next loop (Could even produce a false positive). */
		twosame=0; /* If character is the same with the previous */
	}
    if (c5==x){ /* Will check if current ch makes the password meet the 5th criterion */
        if(prch==ch)twosame=1; /*That would make them 3 same characters in a row(acceptable). */
    }
    
	if ((islower)||(isupper)||(isdigit)){
		if (ch-prch==1){ /* If the 2 last characters are succesional. */
			cc6++;
			if (cc6==3){ /*If 3 succesional characters */
				if(threesuc==1)c6=x+1; /*If threesuc==0 already there's no point in making the check on the next loop(Could even produce a false positive). */
				threesuc=0;
			}		
		}
		else cc6=1; /*Counter starts from 1 again.*/
		
		if(c6==x){ /* Will check if current ch makes the password meet the 6th criterion */
			if(ch-prch==1)threesuc=1; /*That would make them 4 succesional characters in a row(acceptable). */        
		}
    }
	
    if(x==1)printf("Password \""); /* To make it look like the prototype provided */
    printf("%c",ch);
    prch=ch;     /*Saving the previous character. */
    ch = getchar();   /* Read next character */
    
    if (ch=='\n'){    /*If enter is pressed */
      if (x>=MINLENGTH){  /*If password is longer than the set minimum length */
        if(una==0){       /*If there are unacceptable characters */
          if(low+upp+num+spe+twosame+threesuc>=MINCRITERIA) { /* If it meets for criteria than the set minimum */
                if(low*upp*num*spe*twosame*threesuc==1){      /*If all 6 criteria are met */
                  if (x>=2*MINLENGTH)printf("\" is valid (very stong)\n");  /*If password is longer than double the minimum length */
                  else printf("\" is valid (strong)\n");
                }
                else if (low+upp+num+spe+twosame+threesuc==5)printf("\" is valid (fair)\n"); /* If it meets 5 of the criteria */
                else if (clet>=cnum)printf("\" is valid (weak)\n");    /* If there are at least as many letters as numbers */
                else if (clet<cnum)printf("\" is valid (very weak)\n"); /*If there are more numbers than letters */
                
          }
          else printf("\" is invalid (few criteria)\n");
        }else printf("\" is invalid (unacceptable character)\n");
      }  
      else printf("\" is invalid (too short)\n");       
        
      low=0;upp=0;num=0;spe=0;una=0;x=0;clet=0;cnum=0;prch=-1;twosame=1;threesuc=1;c5=0;c6=0;cc6==1; /*Reseting the variable's to their preset values to accept the next password */
      ch=getchar(); /* Since previous character was \n it reads the next one. */

    }
  }
return 0;
}        

 
 
