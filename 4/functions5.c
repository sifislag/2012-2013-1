#include <stdio.h>
#include <stdlib.h>
#include "functions1.h"
#include "functions2.h"
#include "functions3.h"
#include "functions4.h"
#include "functions5.h"


/*Function that calculates a score for each potential gamestatus */
int score(char **c,char n,int diff,int turn,char caller,char enemy){
	/*First four variables are temporary,next 4 are the most "extreme",next 2 are to see how spread(in the right direction) a chain is */
    int imax,imin,jmax,jmin,imaxmax=0,iminmin=n-1,jmaxmax=0,jminmin=n-1,pwidth=0,owidth=0;
    /*Used to store the coordinates of the beginning and end of the strongest black/white chain.*/
    int ibchainmax=0,ibchainmin=n-1,jbchainmax=0,jbchainmin=n-1,iwchainmax=0,iwchainmin=n-1,jwchainmax=0,jwchainmin=n-1;
    /*score is stored in count,variables for the chains(temp and the most extreme),same for what tobelinked() returns. */
    int count=0,i,j,maxpchain=0,maxochain=0,pchain=0,ochain=0,ptblcount=0,maxpcount=0,maxocount=0,otblcount=0,temp=0,mid;
    /*Iteratively goes through the board calling linkedscore() if it finds a cell with the color of the caller or his enemy */
    for(i=0;i<n;i++)
        for(j=0;j<n;j++){
            jmax=0;
            jmin=n-1;
            imax=0;
            imin=n-1;
            if(c[i][j]==caller){
                pchain=1;
                linkedscore(c,n,i,j,&pchain,&imin,&imax,&jmin,&jmax);
                if(caller=='w'){ /*Checks if temporary (imax,...,jmin) counters are closer to the edges than the respective "extreme" ones. */
                    if(imax>imaxmax)
                        imaxmax=imax;
                    if(imin<iminmin)
                        iminmin=imin;
                }else{
                    if(jmax>jmaxmax)
                        jmaxmax=jmax;
                    if(jmin<jminmin)
                        jminmin=jmin;
                }
                if(pchain>=maxpchain){
                    maxpchain=pchain;
                    if(caller=='w'){ /*Changes the maximum width(how spread it is) and the chain "extreme" variables */
                        pwidth=imax-imin;
                        iwchainmax=imax;
                        iwchainmin=imin;
                        jwchainmax=jmax;
                        jwchainmin=jmin;
                    }
                    else{
                        pwidth=jmax-jmin;
                        jbchainmax=jmax;
                        jbchainmin=jmin;
                        ibchainmax=imax;
                        ibchainmin=imin;
                    }
                }
            }
            else if(c[i][j]==enemy){ /*Same process for the caller's enemy */
                ochain=1;
                linkedscore(c,n,i,j,&ochain,&imin,&imax,&jmin,&jmax);
                if(caller=='w'){
                    if(jmax>jmaxmax)
                        jmaxmax=jmax;
                    if(jmin<jminmin)
                        jminmin=jmin;
                }else{
                    if(imax>imaxmax)
                        imaxmax=imax;
                    if(imin<iminmin)
                        iminmin=imin;
                }
                if(ochain>=maxochain){
                    maxochain=ochain;
                    if(caller=='w'){
                        owidth=jmax-jmin;
                        jbchainmax=jmax;
                        jbchainmin=jmin;
                        ibchainmax=imax;
                        ibchainmin=imin;
                    }
                    else{
                        owidth=imax-imin;
                        iwchainmax=imax;
                        iwchainmin=imin;
                        jwchainmax=jmax;
                        jwchainmin=jmin;
                    }
                }
            }
        }
    /*Returns them to their original value */
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            if((c[i][j]=='b'-1)||(c[i][j]=='w'-1))
                c[i][j]++;
        
        /*Winning/losing criteria. */
    if(pwidth==n-1)
        return 10000;
    else if(owidth==n-1)
        return -10000;
    
    /*Adds points to the score that will be returned and removes some if it isn't as spread as a chain of the same size would be in an ideal situation*/
    count+=4*maxpchain-(maxpchain-2*pwidth);
    count-=4*maxochain-(maxochain-2*owidth);
    
    
    
    /*Some checks that require chains to have been formed(so I wait until at least n/4 moves are played by each player)
     *(if a chain is blocked,if a chain is blocking the opponent). */    
    if((turn+1)/2>n/4){
 /*Checks if the biggest chain of one of the players(white) is blocked by cells of the other(black who receives the added points)
  * (gives more points if the blocking cells are away from the end of the chain)*/
        if(iminmin==iwchainmin)/*Only if the white cell with the minimum i is at the same level with the  lowest part of the chain */
            for(i=0;i<iwchainmin;i++){
                if(jwchainmin==0){
                    for(j=jwchainmin;j<=jwchainmin+1;j++){
                        if(c[i][j]=='b')
                            temp+=5*(iwchainmin-i); /*Gives extra points if its closer to the board's edge as opposed to being closer to the chain */
                    }
                }else if(jwchainmin==n-1)
                {
                    for(j=jwchainmin-1;j<=jwchainmin;j++){
                        if(c[i][j]=='b')
                            temp+=5*(iwchainmin-i);
                    }
                }else
                {
                    for(j=jwchainmin-1;j<=jwchainmin+1;j++){
                        if(c[i][j]=='b')
                            temp+=5*(iwchainmin-i);
                    }
                }
            }
        
        if(imaxmax==iwchainmax)
            for(i=iwchainmax+1;i<n;i++){
                if(jwchainmax==0){
                    for(j=jwchainmax;j<=jwchainmax+1;j++){
                        if(c[i][j]=='b')
                            temp+=5*(i-iwchainmax);
                    }
                }
                else if(jwchainmax==n-1)
                {
                    for(j=jwchainmax-1;j<=jwchainmax;j++){
                        if(c[i][j]=='b')
                            temp+=5*(i-iwchainmax);
                    }
                }
                else
                {
                    for(j=jwchainmax-1;j<=jwchainmax+1;j++){
                        if(c[i][j]=='b')
                            temp+=5*(i-iwchainmax);
                    }
                }
            }

        
        
/*If a chain has blocked a certain part of the board its removing points from the player that got blocked*/
/*Removes more points if its closer to the edge of the board that's blocked, and the chain.*/        
        if(iwchainmin==0){
            mid=(jwchainmin+jwchainmax)/2; /*Using it to measure how close it is to the chain(isn't the exact distance but works)*/
            for(i=0;i<iwchainmax;i++){
                for(j=0;j<n;j++){
                    if(c[i][j]=='b'){
                        if(mid-j>0)
                            temp-=2*(iwchainmax-i)*(n-1-(mid-j));
                        else if(mid-j<0)
                            temp-=2*(iwchainmax-i)*(n-1-(j-mid));
                         /*because mid is an average there is a chance j=mid,removes the most points possible because its too close to the chain*/
                    }
                }
            }
        }/*If the biggest white chain is reaching the bottom*/
        else if(iwchainmax==n-1){
            mid=(jwchainmin+jwchainmax)/2;
            for(i=iwchainmin;i<n;i++){
                for(j=0;j<n;j++){
                    if(c[i][j]=='b'){
                        if(mid-j>0)
                            temp-=2*(i-iwchainmin)*(n-1-(mid-j));
                        else
                            temp-=2*(i-iwchainmin)*(n-1-(j-mid));
                    }
                }
            }
        }/*Else the biggest chain has not reached the top or bottom of the board.(more points abstracted if close to the chain and the middle of it)*/
        else{
            mid=(jwchainmin+jwchainmax)/2;
            for(i=iwchainmin;i<=iwchainmax;i++){
                for(j=0;j<n;j++){
                    if(c[i][j]=='b'){
                        if(mid-j>0){
                            if(i-(iwchainmax+iwchainmin)/2>0)
                                temp-=(n-1-(i-(iwchainmax+iwchainmin)/2))*(n-1-(mid-j));
                            else
                                temp-=(n-1-((iwchainmax+iwchainmin)/2-i))*(n-1-(mid-j));
                        }
                        else{
                            if(i-(iwchainmax+iwchainmin)/2>0)
                                temp-=(n-1-(i-(iwchainmax+iwchainmin)/2))*(n-1-(j-mid));
                            else
                                temp-=(n-1-((iwchainmax+iwchainmin)/2-i))*(n-1-(j-mid));
                        }
                    }
                }
            }
        }
        
        if(caller=='w')
            count-=temp;
        else
            count+=temp;
        
        /*Whole process repeated for white */
        
        if(jminmin==jbchainmin)
            for(j=0;j<jbchainmin;j++){
                if(ibchainmin==0){
                    for(i=ibchainmin;i<=ibchainmin+1;i++){
                        if(c[i][j]=='w')
                            temp+=5*(jbchainmin-j);
                    }
                }else if(ibchainmin==n-1)
                {
                    for(i=ibchainmin-1;i<=ibchainmin;i++){
                        if(c[i][j]=='w')
                            temp+=5*(jbchainmin-j);
                    }
                }else
                {
                    for(i=ibchainmin-1;i<=ibchainmin+1;i++){
                        if(c[i][j]=='w')
                            temp+=5*(jbchainmin-j);
                    }
                }
            }
        
        if(jmaxmax==jbchainmin)
            for(j=jbchainmax+1;j<n;j++){
                if(ibchainmax==0){
                    for(i=ibchainmax;i<=ibchainmax+1;i++){
                        if(c[i][j]=='w')
                            temp+=5*(j-jbchainmax);
                    }
                }else if(ibchainmax==n-1)
                {
                    for(i=ibchainmax-1;i<=ibchainmax;i++){
                        if(c[i][j]=='w')
                            temp+=5*(j-jbchainmax);
                    }
                }else
                {
                    for(i=ibchainmax-1;i<=ibchainmax+1;i++){
                        if(c[i][j]=='w')
                            temp+=5*(j-jbchainmax);
                    }
                }
            }
        
        if(jbchainmin==0){
            mid=(ibchainmin+ibchainmax)/2;
            for(j=0;j<jbchainmax;j++){
                for(i=0;i<n;i++){
                    if(c[i][j]=='w'){
                        if(mid-j>0)
                            temp-=2*(jbchainmax-j)*(n-1-(mid-i));
                        else
                            temp-=2*(jbchainmax-j)*(n-1-(i-mid));
                    }
                }
            }
            
        }
        else if(jbchainmax==n-1){
            mid=(ibchainmin+ibchainmax)/2;
            for(j=jbchainmin;j<n;j++){
                for(i=0;i<n;i++){
                    if(c[i][j]=='w'){
                        if(mid-i>0)
                            temp-=2*(j-jbchainmin)*(n-1-(mid-i));
                        else
                            temp-=2*(j-jbchainmin)*(n-1-(i-mid));
                    }
                }
            }
        }
        else{
            mid=(ibchainmin+ibchainmax)/2;
            for(j=jbchainmin;j<=jbchainmax;j++){
                for(i=0;i<n;i++){
                    if(c[i][j]=='w'){
                        if(mid-i>0){
                            if(j-(jbchainmax+jbchainmin)/2>0)
                                temp-=(n-1-(j-(jbchainmax+jbchainmin)/2))*(n-1-(mid-i));
                            else
                                temp-=(n-1-((jbchainmax+jbchainmin)/2-j))*(n-1-(mid-i));
                        }
                        else{
                            if(i-(iwchainmax+iwchainmin)/2>0)
                                temp-=(n-1-(j-(jbchainmax+jbchainmin)/2))*(n-1-(i-mid));
                            else
                                temp-=(n-1-((jbchainmax+jbchainmin)/2-j))*(n-1-(i-mid));
                        }
                    }
                }
            }
        }
        
        
        if(caller=='w')
            count+=temp;
        else
            count-=temp;
    }
    
    
    
    /*Finding the maximum score of tobelinked(best "chain" of cells that can be linked in 2 ways)
     *  for player(one calling the AI) and his opponent.  */
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            if(c[i][j]==caller){
                tobelinked(c,caller,n,i,j,&ptblcount);
                if(ptblcount>maxpcount){
                    maxpcount=ptblcount;
                }
            }
            else if(c[i][j]==enemy){
                tobelinked(c,enemy,n,i,j,&otblcount);
                if(otblcount>maxocount){
                    maxocount=otblcount;
                }
            }
            ptblcount=0;
            otblcount=0;
        }
    }
    /*If the status of the game currently examined by score() is no more than 60% of the point that the statuses could be winning ones(if that makes sense).*/
    /*Gives more value on moves like this(consisting of cells that can be linked in two possible ways).*/
    if((turn/2+diff)<=(3*n)/5){ 
        maxpcount+=maxpcount;
        maxocount+=maxocount;
    }
    count+=2*maxpcount;
    count-=2*maxocount;
    /*Returning the cells set to visited status to their original one */
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            if((c[i][j]=='b'-1)||(c[i][j]=='w'-1))
                c[i][j]++;
     
    
    /*Checking if the way of the "extreme" cells(closer to the edges) is blocked by cells of the opposite color */
    
    if(iminmin!=0){
    	for(j=0;j<n;j++){
    		if(c[iminmin][j]=='w'){
    			if(j!=n-1){ /*If not on the last row check the 2 above it. */
    			    if((c[iminmin-1][j]=='b')&&(c[iminmin-1][j+1]=='b')){
    				    temp+=30;
				    }
				    else if((c[iminmin-1][j]=='b')||(c[iminmin-1][j+1]=='b')){
					    temp+=10;
				    }
				    if(iminmin>1){
					    if(c[iminmin-2][j+1]=='b'){
						    temp+=15;
					    }
				    }
				}
				else{ /*Else check the one above it. */
				    if(c[iminmin-1][j]=='b'){
				    	temp+=15;
					}
				}	
			}
		}  	
	}
	if(imaxmax!=n-1){
    	    for(j=0;j<n;j++){
    		if(c[imaxmax][j]=='w'){
    			if(j!=0){ /*If not on the last row check the 2 above it. */
    			    if((c[imaxmax+1][j]=='b')&&(c[imaxmax+1][j-1]=='b')){
    				    temp+=30;
				    }
				    else if((c[imaxmax+1][j]=='b')||(c[imaxmax+1][j-1]=='b')){
					    temp+=10;
				    }
				    if(imaxmax<n-2){
					    if(c[imaxmax+2][j-1]=='b'){
						    temp+=15;
					    }
				    }
				}
				else{ /*Else check the one above it. */
				    if(c[imaxmax+1][j]=='b'){
				    	temp+=15;
					}
				}	
			}
		}  	
	}
	if(caller=='w'){/*Cells like these lose their value as time passes as the program focuses more on blocking chains than individual cells*/
		count-=temp-turn/2;
	}else{
		count+=temp-turn/2;
	}
	temp=0;
    if(jminmin!=0){
    	for(i=0;i<n;i++){
    		if(c[i][jminmin]=='b'){
    			if(i!=n-1){ /*If not on the last row check the 2 above it. */
    			    if((c[i][jminmin-1]=='w')&&(c[i+1][jminmin-1]=='w')){
    				    temp+=30;
				    }
				    else if((c[i][jminmin-1]=='w')||(c[i+1][jminmin-1]=='w')){
					    temp+=10;
				    }
				    else if(jminmin>1){
					    if(c[i+1][jminmin-2]=='w'){
						    temp+=15;
					    }
				    }
				}
				else{ /*Else check the one above it. */
				    if(c[i][jminmin-1]=='w'){
				    	temp+=15;
					}
				}	
			}
		}  	
	}
	if(jmaxmax!=n-1){
    	    for(i=0;i<n;i++){
    		if(c[i][jmaxmax]=='b'){
    			if(i!=0){ /*If not on the last row check the 2 above it. */
    			    if((c[i][jmaxmax+1]=='w')&&(c[i-1][jmaxmax+1]=='w')){
    				    temp+=30;
				    }
				    else if((c[i][jmaxmax+1]=='w')||(c[i-1][jmaxmax+1]=='w')){
					    temp+=10;
				    }
				    if(jmaxmax<n-2){
					    if(c[i-1][jmaxmax+2]=='w'){
						    temp+=20;
					    }
				    }
				}
				else{ /*Else check the one above it. */
				    if(c[i][jmaxmax+1]=='w'){
				    	temp+=15;
					}
				}	
			}
		}  	
	}
	if(caller=='w'){
		count+=temp-turn/2;
	}else{
		count-=temp-turn/2;
	}
	return count;
     
}


