#include <stdio.h>
#include <math.h>
#define MAXNUM 400000
#define MAXM 6

main() {
    int n,m,i,k,cpn,ctwo,cdpn,cper,cmul,csup;
    int tempsum,sum;
    cper=0;                                                   /*Counter for perfect numbers.*/
    cmul=0;                                                   /*Counter for multiperfect numbers.*/
    csup=0;                                                   /*Counter for superperfect numbers.*/
    cpn=0;                                                    /*Counter for (m-k) numbers */
    cdpn=0;                                                   /*Counter for different(m-k) perfect numbers */

    for (n=2;n<=MAXNUM;n++){
        sum=n;                                               /*Summary given the number's value before starting to look for divisor summaries. */
        ctwo=0;                                              /*Counter helping find out if a number is (m-k) perfect more than once.*/
        for (m=1;m<=MAXM;m++){
            tempsum=sum;                                      /*Temporary virable for summary,sum will be used in the repeat construct conditions. */
            for (i=2;i*i<=sum;i++){                           /*Finding the summary of the divisors. */
                if(sum %i ==0){  
                    tempsum +=i;
                    if( i*i != sum)tempsum +=sum/i;
                    }}
            sum=tempsum+1;                                  /*Adding the number 1 to the divisor summary.*/
           
                if(sum % n==0){                             /*Checking if the division of the summary and the number has a remainder. */
                    if (ctwo==0){cdpn++;}                   
                    cpn++;
                    ctwo++;
                    k=sum/n;
                    if(m==1){                               /*Condition for multiperfect(and perfect) numbers.*/
                        cmul++;
                        if (k==2){                          /*Second condition of perfect numbers*/
                            printf("%d  is a (%d-%d)-perfect number,that is a perfect number.\n",n,m,k);                        
                            cper++;
                        }else{                              /*Ones meeting only the first condition are only multiperfect*/
                            printf("%d  is a (%d-%d)-perfect number,that is a multiperfect number.\n",n,m,k);}
                }
                    else if((m==2)&&(k==2)){                /*Condition of superperfect numbers.*/
                        csup++;
                        printf("%d  is a (%d-%d)-perfect number,that is a superperfect number.\n",n,m,k);}
                    else{                                  /*Rest are simple (m-k)-perfect numbers.*/
                        printf("%d  is a (%d-%d)-perfect number.\n",n,m,k);}
            }
        }
    }
    
printf("\nFound %d distinct (m-k)-perfect numbers(%5.4f%% of 400000) in %d occurancies.\n",cdpn,100*(((double)cdpn)/400000),cpn);
printf("Found %d perfect numbers.\n",cper);
printf("Found %d multiperfect numbers(including perfect numbers).\n",cmul);
printf("Found %d superperfect numbers.\n",csup);
} 
