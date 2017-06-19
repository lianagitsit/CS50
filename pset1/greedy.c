#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    float inputChangeOwed;
    
    //ask how much change is owed
    // validate amount (non-negative) and re-prompt until valid reply
    do 
    {
        printf("How much change is owed?: ");
        inputChangeOwed = get_float();
    }
    while (inputChangeOwed < 0);
    
    //convert dollars to cents (*100)
    float changeInCents = inputChangeOwed * 100;
    
    //int intChange = (int) changeInCents;
    //printf("%i\n", intChange);
    //THIS JUST LOPS OFF THE DECIMAL TO MAKE AN INT
    
    //round to nearest penny, which is dumb bc of floating pt imprecision
    //round function rounds to nearest int rather than just lopping off the decimal
    int correctIntChange = round(changeInCents);
    //printf("Change owed in cents: %i\n", correctIntChange);
    
    //return minimum number of coins that can be used
    //always use the largest coins possible
   
    //correctIntChange -= coin
    
    int countCoinsToUser = 0;
    
    int shadowCount =0;
    
    int quarters = correctIntChange / 25;
    shadowCount += quarters;
    //printf("Oh! I need %i quarters!\n", quarters);
    int changeLeft = correctIntChange % 25;
    //printf("And I have %i cents leftover for other coins\n", changeLeft);
    
    int dimes = changeLeft / 10;
    shadowCount += dimes;
    //printf("Dope! I then give %i dimes!\n", dimes);
    int newChangeLeft = changeLeft % 10;
    //printf("Kickin' rad! I have %i cents leftover for other coins\n", newChangeLeft);
    
    int nickels = newChangeLeft / 5;
    shadowCount += nickels;
    //printf("Woo! I then give %i nickels!\n", nickels);
    int postNickelsChange = newChangeLeft % 5;
    //printf("And now just %i left to give as pennies!\n", postNickelsChange);
    
    //printf("So, like I just said, you can have %i pennies too!\n", postNickelsChange);
    shadowCount += postNickelsChange;
    printf("%i\n", shadowCount);
    
    
     
    while (correctIntChange / 25 >= 1) 
    {
        correctIntChange = correctIntChange - 25;
        countCoinsToUser += 1;
    }
    
    //printf("%i\n", correctIntChange);
    //printf("%i\n", countCoinsToUser);
        
    while (correctIntChange / 10 >= 1)
    {
        correctIntChange -=10;
        countCoinsToUser += 1;
    }
        
      while (correctIntChange / 5 >= 1)
    {
        correctIntChange -=5;
        countCoinsToUser += 1;
    }
    
      while (correctIntChange / 1 >= 1)
    {
        correctIntChange -=1;
        countCoinsToUser += 1;
    }
    
    //printf("%i\n", correctIntChange);
    //printf("%i\n", countCoinsToUser);
    
    //keep track of coins used
        //and how many still owed
}
